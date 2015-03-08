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
object *new(unsigned int id, char *name) {
	object *en = NULL;
	if (growArray(&en, 1, sizeof(*en)) != 0) {
		perror("failed to grow array");
	}
	en->id = id;
	en->name = strndup(name, BUFSIZ);
#ifdef DEBUG
	assert(en->name != NULL);
#endif
	return en;
}

object *set(object *base, object *en) {
#ifdef DEBUG
	assert(en != NULL);
#endif

	if (base == NULL) {
		base = new(0, "(null)");
	}

	base->name = strndup(en->name, BUFSIZ);
#ifdef DEBUG
	assert(base->name != NULL);
#endif

	base->id = en->id;
	return base;
}

char *getName(object *en) {
	return (en->name);
}

int cmpName(const void *a, const void *b) {
	object *en1 = (object *)a;
	object *en2 = (object *)b;
	return (strcasecmp(en1->name, en2->name));
}

int cmpId(const void *a, const void *b) {
	object *en1 = (object *)a;
	object *en2 = (object *)b;
	return ((int)en2->id - (int)en1->id);
}

void setName(char *name, object *en) {
#ifdef DEBUG
	assert(en != NULL && name != NULL);
#endif
	en->name = strndup(name, BUFSIZ);
#ifdef DEBUG
	assert(en->name != NULL);
#endif
}

int isClean(ArrayList *list) {
	foreach (list) {
		if (list->obj != NULL) {
			if (list->obj->name != NULL) {
				printf("entry %u not freed\n", list->obj->id);
				return -1;
			}
		}
	}
	return 0;
}

void *toArray(ArrayList *list) {
	char **array = NULL;
	unsigned int i = 0;
	size_t listSize = getSize(list);
	if (listSize == 0) {
		return NULL;
	}
	if (growArray(&array, listSize, sizeof(char *)) != 0) {
		perror("failed to grow array");
	}
	if (array == NULL) {
		perror("array == NULL");
		return NULL;
	}
	foreach (list) {
		array[i] = strndup(list->obj->name, BUFSIZ);
#ifdef DEBUG
		assert(array[i] != NULL);
#endif
		i++;
	}
	return array;
}

void toString(object *obj) {
	printf("%02u -> %s\n", obj->id, obj->name);
}

void cleanList(ArrayList *list) {
	foreach (list) {
		list->obj->name = cleanArray((char **)list->obj->name, NULL);
#ifdef DEBUG
		assert(list->obj->name == NULL);
#endif
		rm(list, list->obj);
	}
}

