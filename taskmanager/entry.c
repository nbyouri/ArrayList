#include <assert.h>
#include "list.h"
#include "tools.h"

/*
 *
 *
 * Fonctions à changer, elles dépendent du contenu
 * de la liste.
 *
 *
 *
 */
object *new(unsigned int id) {
	object *en = NULL;
	en = malloc(sizeof(object));
	if (en == NULL) {
		perror("failed to grow array");
	}
	en->id = id;
	return en;
}

object *set(object *base, object *obj) {
#ifdef DEBUG
	assert(obj != NULL);
#endif

	if (base == NULL) {
		base = new(0);
	}

        /* populate fields */
        base->id = obj->id;
        base->uid = obj->uid;
        strlcpy(base->uid_name, obj->uid_name, sizeof(obj->uid_name));
        base->pid = obj->pid;
        base->ppid = obj->ppid;
        strlcpy(base->name, obj->name, sizeof(obj->name));
        strlcpy(base->cmdline, obj->cmdline, sizeof(obj->cmdline));
        strlcpy(base->state, obj->state, sizeof(obj->state));
        base->cpu_user = obj->cpu_user;
        base->cpu_system = obj->cpu_system;
        base->vsz = obj->vsz;
        base->rss = obj->rss;
        base->prio = obj->prio;

	return base;
}

int cmpId(const void *a, const void *b) {
	object *en1 = (object *)a;
	object *en2 = (object *)b;
	return ((int)en2->id - (int)en1->id);
}


void toString(object *obj) {
	if (obj == NULL) {
		printf("object is null\n");
		return;
	}
	printf("%-6u : %-6s : %-6s : %s\n", obj->pid, obj->uid_name, obj->state,
            obj->cmdline);
}

void cleanList(ArrayList *list) {
	/* remove elements */
	while (!isEmpty(list)) {
		list->obj = TAILQ_FIRST(list->head);
		TAILQ_REMOVE(list->head, list->obj, entries);
	}
	/* free token element */
	free(list->obj);
	list->obj = NULL;
}

