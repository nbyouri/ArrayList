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
	en = growArray(en, 1, sizeof(*en));
	en->id = id;
	en->name = strndup(name, BUFSIZ);
	return en;
}

object *set(object *base, object *en) {
#ifdef DEBUG
	assert(en != NULL);
#endif

	if (base == NULL) {
		base = new(0, "(null)");
	}

	if (en->name != NULL) {
		strlcpy(base->name, en->name, BUFSIZ);
	} else {
		base->name = NULL;
	}

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

ssize_t setName(char *name, object *en) {
#ifdef DEBUG
	assert(en != NULL && name != NULL);
#endif
	en->name = strndup(name, BUFSIZ);
	return -1;
}

void isClean(void) {
	foreach (np) {
		assert(np != NULL);
		assert(np->name != NULL);
	}
}

void *toArray() {
	char **array = NULL;
	unsigned int i = 0;
	size_t listSize = getSize();
	if (listSize == 0) {
		return NULL;
	}
	array = growArray(array, listSize, sizeof(char *));
	if (array == NULL) {
		perror("array == NULL");
		return NULL;
	}
	foreach (np) {
		array[i] = strndup(np->name, BUFSIZ);
		i++;
	}
	return array;
}

void toString(object *en) {
	printf("%02u -> %s\n", en->id, en->name);
}

void cleanList() {
	foreach (np) {
		np->name = cleanPtr((char **)np->name, NULL);
#ifdef DEBUG
		assert(np->name == NULL);
#endif
		rm(np);
	}
}

