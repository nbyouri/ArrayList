#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <pwd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/sched.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <kvm.h>
#include <sys/proc.h>
#include <sys/vmmeter.h>
#include <sys/swap.h>
#include "tools.h"
#include "list.h"

extern int errno;

char    *state_abbrev[] = {
        "", "start", "run", "sleep", "stop", "zomb", "dead", "onproc"
};

int sortPid(const void *a, const void *b) {
        return (((object *)a)->pid - ((object *)b)->pid);
}

void get_task_list(ArrayList * list) {
#ifdef __OpenBSD__
	struct kinfo_proc *kp = NULL;
#else
        struct kinfo_proc2 *kp = NULL;
#endif

        struct passwd *passwdp;
        char **args = NULL;
        char *buf = NULL;
        unsigned int i = 0;
        int nproc = 0;
        //size_t size = sizeof(*kp);


        /* get process number, stolen from ps */
        /* example with kvm functions */
        char errbuf[_POSIX2_LINE_MAX];
        kvm_t *kdp = kvm_openfiles(NULL, NULL, NULL, KVM_NO_FILES, errbuf);
        if (kdp == NULL)
                errx(1, "%s", errbuf);

        /* fill kinfo_proc2 structure */
        /* XXX does it return sleeping processes ? */
#ifdef __OpenBSD__
	if (!(kp = kvm_getprocs(kdp, KERN_PROC_ALL, 0,
	        	sizeof(struct kinfo_proc), &nproc)))
#else
        if (!(kp = kvm_getproc2(kdp, KERN_PROC_ALL, 0,
                    sizeof(struct kinfo_proc2), &nproc)))
#endif
                err(1, "%s", kvm_geterr(kdp));

        for (i = 0; i < nproc; i++) {
                /* get per-process information in our entry */
#ifdef __OpenBSD__
		struct kinfo_proc p = kp[i];
#else 
                struct kinfo_proc2 p = kp[i];
#endif

                list->obj = new(i);

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
                if (!P_ZOMBIE(&p)) { /* XXX much less relax than OpenBSD's */
                //if (!(p.p_stat == SDEAD)) {
                        /* get process args */
                        /* example with kvm functions */

#ifdef __OpenBSD__
			args = kvm_getargv(kdp, &kp[i], BUFSIZ);
#else
                        args = kvm_getargv2(kdp, &kp[i], BUFSIZ);
#endif
                        /* example with sysctl directly */
                        /*
                        mib[0] = CTL_KERN;
                        mib[1] = KERN_PROC_ARGS;
                        mib[2] = list->obj->pid;
                        mib[3] = KERN_PROC_ARGV;

                        if (sysctl(mib, 4, NULL, &size, NULL, 0) < 0)
                                errx(1, "failed to get kern.proc_args");

                        args = malloc(size * sizeof(char *));
                        for (int i = 0; i < size; i++) {
                                args[i] = malloc(BUFSIZ);
                        }

                        strcpy(args[0], "test");
                        if (sysctl(mib, 4, args, &size, NULL, 0) < 0)
                                errx(1, "failed to get kern.proc_args");
                        size = 5 *  size / 4;
                        */

                        if (args != NULL) {
                                buf = strjoin(" ", args);
                                /* XXX free args */
                                strlcpy(list->obj->cmdline, buf,
                                    sizeof(list->obj->cmdline));
                        }
                        list->obj->cpu_user = (100.0 *
                            ((double) p.p_pctcpu / FSCALE));
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
        //if (kvm_close(kdp) == -1)
        //        errx(1, "failed to close kvm");
 }

int sleeping(unsigned int pid) {
        int mib[6];
#ifdef __OpenBSD__
        struct kinfo_proc kp;
#else	
	struct kinfo_proc2 kp;
#endif
        size_t size = sizeof(kp);
        mib[0] = CTL_KERN;
#ifdef __OpenBSD__
        mib[1] = KERN_PROC;
#else
	mib[1] = KERN_PROC2;
#endif
        mib[2] = KERN_PROC_PID;
        mib[3] = pid;
        mib[4] = size;
        mib[5] = 1;

        if (sysctl(mib, 6, &kp, &size, NULL, 0) < 0)
                errx(1, "could not read kern.proc2 for pid %u\n", pid);

        return (kp.p_stat == SSTOP);
}

/* needs big timeout */
int get_cpu_usage(short *cpu_count, float *cpu_user, float *cpu_system) {
        static long cur_user = 0, cur_system = 0, cur_total = 0;
        static long old_user = 0, old_system = 0, old_total = 0;

#ifdef __OpenBSD__
        int mib[] = { CTL_KERN, KERN_CPTIME };
#else
	int mib[] = { CTL_KERN, KERN_CP_TIME };
#endif
        long cp_time[CPUSTATES];
        size_t size = sizeof(cp_time);

        if (sysctl(mib, 2, &cp_time, &size, NULL, 0) < 0)
                errx(1, "failed to get kern.cp_time");

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

int get_memory_usage (uint64_t *memory_total, uint64_t *memory_free, uint64_t *memory_cache, uint64_t *memory_buffers, uint64_t *swap_total, uint64_t *swap_free)
{
        int mib[] = {CTL_VM, VM_METER};
        struct vmtotal vmtotal;

        struct swapent *swdev;
        int nswap, i;
        size_t size;

        size = sizeof(vmtotal);
        if (sysctl(mib, 2, &vmtotal, &size, NULL, 0) < 0)
                errx(1,"failed to get vm.meter");
        /* cheat : rm = tot used, add free to get total */
        *memory_total = pagetok(vmtotal.t_rm + vmtotal.t_free);
        *memory_free = pagetok(vmtotal.t_free);
        *memory_cache = 0;
        *memory_buffers = pagetok(vmtotal.t_rm - vmtotal.t_arm);

        /* get swap stats */
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
        for (i = 0; i < nswap; i++) {
                if (swdev[i].se_flags & SWF_ENABLE) {
                        *swap_free += (swdev[i].se_nblks - swdev[i].se_inuse);
                        *swap_total += swdev[i].se_nblks;
                }
        }
        *swap_total *= DEV_BSIZE;
        *swap_free *= DEV_BSIZE;
        free(swdev);
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
                sleep(3);
        }


        /*
         * Get memory stats, works.
         *
         */
        printf("\n\nMEM_INFO\n\n");
        uint64_t memory_total, memory_free, memory_cache, memory_buffers, swap_total, swap_free;
        get_memory_usage(&memory_total, &memory_free, &memory_cache,
            &memory_buffers, &swap_total, &swap_free);
        printf("memory_total = %llu, memory_free = %llu, memory_cache = %llu\n"
            "memory_buffers = %llu, swap_total = %llu, swap_free = %llu\n",
            memory_total, memory_free, memory_cache,
            memory_buffers, swap_total, swap_free);

	puts("finished...");
        return 0;

}
