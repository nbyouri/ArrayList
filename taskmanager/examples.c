#include <dirent.h>
#include <inttypes.h>
#include <stdbool.h>
#include <err.h>
#include <errno.h>
#include <pwd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <paths.h>
#include <sys/sched.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <kvm.h>
#ifndef __DragonFly__
#include <sys/proc.h>
#else
#include <sys/user.h>
#include <kinfo.h>
#endif
#include <sys/vmmeter.h>
#include <fcntl.h>
#ifndef __DragonFly__
#include <sys/swap.h>
#endif
#include "tools.h"
#include "list.h"

extern int errno;

#ifdef __DragonFly__
#define SDEAD	208
#endif

char    *state_abbrev[] = {
        "", "start", "run", "sleep", "stop", "zomb", "dead", "onproc"
};

int sortPid(const void *a, const void *b) {
        return (((object *)a)->pid - ((object *)b)->pid);
}

void get_task_list(ArrayList * list) {
#ifdef __NetBSD__
	struct kinfo_proc2 *kp = NULL;
#else
        struct kinfo_proc *kp = NULL;
#endif

        struct passwd *passwdp;
        char **args = NULL;
        char *buf = NULL;
        unsigned int i = 0;
        int nproc = 0;


        /* get process number, stolen from ps */
        /* example with kvm functions */
        char errbuf[_POSIX2_LINE_MAX];
        kvm_t *kdp = kvm_openfiles(_PATH_DEVNULL, NULL, NULL, O_RDONLY, errbuf);
        if (kdp == NULL)
                errx(1, "%s", errbuf);

        /* fill kinfo_proc2 structure */
        /* XXX does it return sleeping processes ? */
#ifdef	__OpenBSD__
	if (!(kp = kvm_getprocs(kdp, KERN_PROC_ALL, 0,
	            sizeof(struct kinfo_proc), &nproc)))
#elif __DragonFly__
	if (!(kp = kvm_getprocs(kdp, KERN_PROC_ALL, 0, &nproc)))
#else /* probably NetBSD or MirBSD */
        if (!(kp = kvm_getproc2(kdp, KERN_PROC_ALL, 0,
                    sizeof(struct kinfo_proc2), &nproc)))
#endif
                err(1, "%s", kvm_geterr(kdp));

        for (i = 0; i < nproc; i++) {
#ifdef	__DragonFly__
		if (kp[i].kp_pid >= PID_MAX || kp[i].kp_pid < 1)
			continue; /* not valid */
#else
		if (kp[i].p_pid < 1)
			continue; /* not valid */
#endif

                /* get per-process information in our entry */
#ifdef __NetBSD__
		struct kinfo_proc2 p = kp[i];
#else
                struct kinfo_proc p = kp[i];
#endif

                list->obj = new(i);

#ifdef	__DragonFly__
                list->obj->pid = p.kp_pid;
                list->obj->ppid = p.kp_ppid;
                list->obj->uid = p.kp_uid;
                list->obj->prio = p.kp_nice;
                list->obj->vsz = p.kp_vm_dsize + p.kp_vm_ssize + p.kp_vm_tsize;
                list->obj->vsz *= getpagesize();
                list->obj->rss = p.kp_vm_rssize * getpagesize();
                snprintf(list->obj->state, sizeof(list->obj->state), "%s",
                    state_abbrev[p.kp_stat]);
                strlcpy(list->obj->name, p.kp_comm, strlen(p.kp_comm) + 1);
#else
                list->obj->pid = p.p_pid;
                list->obj->ppid = p.p_ppid;
                list->obj->uid = p.p_uid;
	        list->obj->prio = p.p_priority - PZERO;
                list->obj->vsz = p.p_vm_dsize + p.p_vm_ssize + p.p_vm_tsize;
                list->obj->vsz *= getpagesize();
                list->obj->rss = p.p_vm_rssize * getpagesize();
                snprintf(list->obj->state, sizeof(list->obj->state), "%s",
                    state_abbrev[p.p_stat]);
                strlcpy(list->obj->name, p.p_comm, strlen(p.p_comm) + 1);
#endif
#ifdef __DragonFly__
                if (!(p.kp_stat == SDEAD)) { /* same as OpenBSD P_ZOMBIE */
#else
                if (!(p.p_stat == SDEAD)) {
#endif
                        /* get process args */
#if defined(__OpenBSD__) || defined(__DragonFly__)
			args = kvm_getargv(kdp, &kp[i], BUFSIZ);
#else
                        args = kvm_getargv2(kdp, &kp[i], BUFSIZ);
#endif
                        if (args != NULL) {
                                buf = strjoin(" ", args);
                                /* XXX free args */
                                strlcpy(list->obj->cmdline, buf,
                                    sizeof(list->obj->cmdline));
                        }
#ifdef __DragonFly__
			list->obj->cpu_user = 0; /* TODO */
#else
                        list->obj->cpu_user = (100.0 *
                            ((double) p.p_pctcpu / FSCALE));
#endif
                        list->obj->cpu_system = 0; /* XXX TODO */
                        passwdp = getpwuid(list->obj->uid);
                        if (passwdp != NULL && passwdp->pw_name != NULL)
                                strlcpy(list->obj->uid_name, passwdp->pw_name,
                                    sizeof(list->obj->uid_name));
                        /* add entry to list */
                        add(list, list->obj);
                }
        }
        sort(list, sortPid);
        free(buf);
        buf = NULL;
        free(kp);
        kp = NULL;
 }

bool sleeping(unsigned int pid) {
	kvm_t	*kd;
#ifdef __NetBSD__
        struct kinfo_proc2 *kp = NULL;
#else
	struct kinfo_proc *kp = NULL;
#endif
	int cnt;
	int ret;

	kd = kvm_openfiles(_PATH_DEVNULL, _PATH_DEVNULL, NULL, O_RDONLY, NULL);
	if (kd == NULL)
		return false;

#ifdef __OpenBSD__
        kp = kvm_getprocs(kd, KERN_PROC_PID, pid, 0, &cnt);
#elif __DragonFly__
	kp = kvm_getprocs(kd, KERN_PROC_PID, pid, &cnt);
#else /* probably NetBSD */
	kp = kvm_getproc2(kd, KERN_PROC_PID, pid,
            sizeof(struct kinfo_proc2), &cnt);
#endif
	if (kp == NULL) {
		kvm_close(kd);
		return false;
	}

#if __DragonFly__
	ret = (kp->kp_stat == SSTOP);
#else
	ret = (kp->p_stat == SSTOP);
#endif
	kvm_close(kd);
	return ret;
}

/* needs big timeout */
int get_cpu_usage(short *cpu_count, float *cpu_user, float *cpu_system) {
        static long cur_user = 0, cur_system = 0, cur_total = 0;
        static long old_user = 0, old_system = 0, old_total = 0;
	uint64_t cp_time[CPUSTATES];
#ifdef	__DragonFly__
       int mib[2];
       struct kinfo_cputime percpu;
       size_t size = sizeof(percpu);

       if (sysctlbyname("kern.cputime", &percpu, &size, NULL, 0) < 0)
               errx(1, "failed to sysctl kern.cputime");

       cp_time[CP_USER] = percpu.cp_user;
       cp_time[CP_NICE] = percpu.cp_nice;
       cp_time[CP_SYS]  = percpu.cp_sys;
       cp_time[CP_INTR] = percpu.cp_intr;
       cp_time[CP_IDLE] = percpu.cp_idle;
#else
#ifdef	__OpenBSD__
        int mib[] = { CTL_KERN, KERN_CPTIME };
#elif	__NetBSD__
	int mib[] = { CTL_KERN, KERN_CP_TIME };
#endif
        size_t size = sizeof(cp_time);
        if (sysctl(mib, 2, &cp_time, &size, NULL, 0) < 0)
                errx(1, "failed to get kern.cp_time");

#endif
        old_user = cur_user;
        old_system = cur_system;
        old_total = cur_total;

        cur_user = cp_time[CP_USER] + cp_time[CP_NICE];
        cur_system = cp_time[CP_SYS] + cp_time[CP_INTR];
        cur_total = cur_user + cur_system + cp_time[CP_IDLE];

        *cpu_user = (old_total > 0) ?
            (cur_user - old_user) * 100 / (double)(cur_total - old_total) : 0;

        *cpu_system = (old_total > 0) ?
            (cur_system - old_system) * 100 / (double)(cur_total - old_total) : 0;

        /* get #cpu */
        size = sizeof(&cpu_count);
        mib[0] = CTL_HW;
        mib[1] = HW_NCPU;
        if (sysctl(mib, 2, cpu_count, &size, NULL, 0) == -1)
                errx(1,"failed to get cpu count");

        return 0;
}

/* vmtotal values in #pg */
#define pagetok(nb) ((nb) * (getpagesize()))

int get_memory_usage (uint64_t *memory_total, uint64_t *memory_free,
    uint64_t *memory_cache, uint64_t *memory_buffers, uint64_t *swap_total,
    uint64_t *swap_free) {
#ifdef __OpenBSD__
	int mib[] = {CTL_VM, VM_UVMEXP};
	struct uvmexp uvmexp;
#else
#if __NetBSD__
        int mib[] = {CTL_VM, VM_METER};
        struct vmtotal vmtotal;
#endif
#endif

#ifndef __DragonFly__
        struct swapent *swdev;
        int nswap;
#endif
        size_t size;

#ifdef __OpenBSD__
	size = sizeof(uvmexp);
	if (sysctl(mib, 2, &uvmexp, &size, NULL, 0) < 0)
		errx(1,"failed to get vm.uvmexp");
	/* cheat : rm = tot used, add free to get total */
	*memory_free = pagetok((uint64_t)uvmexp.free);
	*memory_total = pagetok((uint64_t)uvmexp.npages);
	*memory_cache = 0;
	*memory_buffers = 0; /*pagetok(uvmexp.npages - uvmexp.free - uvmexp.active);*/
#else
        size = sizeof(vmtotal);
#ifdef __DragonFly__
	if (sysctlbyname("vm.vmstats", &vmtotal, &size, NULL, 0) < 0)
#else
        if (sysctl(mib, 2, &vmtotal, &size, NULL, 0) < 0)
#endif
                errx(1,"failed to get vm.meter");
        /* cheat : rm = tot used, add free to get total */
        *memory_total = pagetok(vmtotal.t_rm + vmtotal.t_free);
        *memory_free = pagetok(vmtotal.t_free);
        *memory_cache = 0;
        *memory_buffers = pagetok(vmtotal.t_rm - vmtotal.t_arm);
#endif

        /* get swap stats */
#ifndef __DragonFly__
        if ((nswap = swapctl(SWAP_NSWAP, 0, 0)) == 0)
                errx(1,"failed to get swap device count");

        if ((swdev = calloc(nswap, sizeof(*swdev))) == NULL)
                errx(1,"failed to allocate memory for swdev structures");

        if (swapctl(SWAP_STATS, swdev, nswap) == -1) {
                free(swdev);
                errx(1,"failed to get swap stats");
        }
	/* Total things up */
	*swap_total = *swap_free = 0;
	int i;
	for (i = 0; i < nswap; i++) {
		if (swdev[i].se_flags & SWF_ENABLE) {
			*swap_free += (swdev[i].se_nblks - swdev[i].se_inuse);
			*swap_total += swdev[i].se_nblks;
		}
	}
	*swap_total *= DEV_BSIZE;
	*swap_free *= DEV_BSIZE;
	free(swdev);
#else /* DragonFly */
	int temp_swap_total, temp_swap_used;
	size_t len = sizeof(int);

	if (sysctlbyname("vm.swap_size", &temp_swap_total, &len, NULL, 0) < 0)
		errx(1, "failed to get vm.swap_size");

	if (sysctlbyname("vm.swap_anon_use", &temp_swap_used, &len, NULL, 0) < 0)
		errx(1, "failed to get vm.swap_anon_use");

#define CONVERT(v)     ((quad_t)(v) * (getpagesize() / 1024))

	*swap_total = CONVERT(temp_swap_total);
	*swap_free = CONVERT(temp_swap_total - temp_swap_used);


#endif
	return 0;
}

int main(void) {
	/*
	 *
	 *
	 * Get task list and
	 * assign to a list.
	 *
	 * works.
	 */
	ArrayList *list;

	list = malloc(sizeof(*list));

	initList(list);

	get_task_list(list);

	printf("\n\nPROC_LIST\n\n");
	foreach(list) {
		toString(list->obj);
		/*
		 *
		 * is a pid sleeping ?
		 *
		 * not sure if working.
		 *
		 */
		printf("-> sleeping ? %s\n", sleeping(list->obj->pid) ? "yes" : "no");
	}

	cleanList(list);

	if (!isEmpty(list)) {
		printf("list is supposed to be empty!\n");
	}

	free(list);
	list = NULL;

	/*
	 *
	 * Get cpu usage info.
	 *
	 * seems to work, verify values.
	 */
	printf("\n\nCPU_INFO\n\n");
	short cpu_count = 0;
	float cpu_user = 0;
	float cpu_system = 0;

	for (int i = 0; i < 10; i++) {
		get_cpu_usage(&cpu_count, &cpu_user, &cpu_system);
		printf("cpu_count = %d, cpu_user = %f, cpu_system = %f\n\n", cpu_count,
		    cpu_user, cpu_system);
		sleep(2);
	}


	/*
	 * Get memory stats, works.
	 *
	 */
	printf("\n\nMEM_INFO\n\n");
	uint64_t memory_total, memory_free, memory_cache, memory_buffers, swap_total, swap_free;
	get_memory_usage(&memory_total, &memory_free, &memory_cache,
	    &memory_buffers, &swap_total, &swap_free);
	printf("memory_total = %" PRIu64 " memory_free = %" PRIu64 ", memory_cache = %" PRIu64 "\n"
	    "memory_buffers = %" PRIu64 ", swap_total = %" PRIu64 ", swap_free = %" PRIu64 "\n",
	    memory_total, memory_free, memory_cache,
	    memory_buffers, swap_total, swap_free);

	puts("finished...");
	return 0;

}
