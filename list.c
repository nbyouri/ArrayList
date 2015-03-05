#include "list.h"
#include "strings.h"

#ifdef DEBUG
#include <assert.h>
#endif

void sort(int (*cmp)(const void *, const void *)) {
	unsigned int i = 0;
	for (i = 0; i < getSize(); i++) {
		foreach(np) {
			object *next = getNext(np);
			if (next != NULL) {
				if (cmp(np, next) > 0) {
					swap(np, next);
				}
			}
		}
	}
}

void *toArray() {
	char **array = NULL;
	unsigned int i = 0;
	array = growArray(array, getSize(), sizeof(char *));
	if (array == NULL) {
		perror("*array == NULL");
	}
	foreach (np) {
		array[i] = growArray(array[i], BUFSIZ, sizeof(char));
		strlcpy(array[i], np->name, BUFSIZ);
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
		assert(np->name == NULL);
		rm(np);
	}
}

void add(object *en) {
	assert(en != NULL);
	TAILQ_INSERT_TAIL(&head, en, entries);
}

void addOnTop(object *en) {
	assert(en != NULL);
	TAILQ_INSERT_HEAD(&head, en, entries);
}

void addBefore(object *base, object *new) {
	assert(base != NULL && new != NULL);
	TAILQ_INSERT_BEFORE(base, new, entries);
}

void addAfter(object *base, object *new) {
	assert(base != NULL && new != NULL);
	TAILQ_INSERT_AFTER(&head, base, new, entries);
}

void rm(object *en) {
	assert(en != NULL);
	TAILQ_REMOVE(&head, en, entries);
}

object *new(unsigned int id, char *name) {
	object *en = NULL;
	en = growArray(en, 1, sizeof(*en));
	en->id = id;
	en->name = growArray(en->name, BUFSIZ, sizeof(*(en->name)));
	if (name != NULL) {
		strlcpy(en->name, name, BUFSIZ);
	}
	return en;
}

object *get(unsigned int i) {
	foreach (np) {
		if (np->id == i) {
			return np;
		}
	}
	return NULL;
}

object *getFirst(void) {
	return TAILQ_FIRST(&head);
}

object *getLast(void) {
	return TAILQ_LAST(&head, tailhead);
}

object *getPrev(object *en) {
	assert(en != NULL);
	object *prev;
	prev =  TAILQ_PREV(en, tailhead, entries);
	return prev;
}

object *getNext(object *en) {
	assert(en != NULL);
	object *next;
	next = TAILQ_NEXT(en, entries);
	return next;
}

object *set(object *base, object *en) {
	assert(en != NULL);

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

void setPrev(object *base, object *en) {
	assert(base != NULL && en != NULL);
	object *prev = getPrev(base);
	if (prev != NULL) {
		set(prev, en);
	}
}

void setNext(object *base, object *en) {
	assert(base != NULL && en != NULL);
	object *next = getNext(base);
	if (next != NULL) {
		set(next, en);
	}
}

char *getName(object *en) {
	return (en->name);
}

unsigned int getId(object *en) {
	assert(en != NULL);
	return en->id;
}

ssize_t setName(char *name, object *en) {
	assert(en != NULL && name != NULL);
	if (en->name == NULL) {
		en->name = growArray(en->name, BUFSIZ, sizeof(char));
	}
	if (strlen(name) > 0) {
		return (ssize_t)strlcpy(en->name, name, BUFSIZ);
	}
	return -1;
}

size_t getSize(void) {
	size_t size = 0;
	foreach (np) {
		size++;
	}
	return size;
}

int isEmpty(void) {
	return TAILQ_EMPTY(&head);
}

void swap(object *first, object *second) {
	assert(first != NULL && second != NULL);
	if (first->id != second->id) {
		object *temp = NULL;
		temp = set(temp, first);

		first = set(first, second);
		second = set(second, temp);

		temp = cleanPtr((char **)temp, NULL);
	}
}

void swapNext(object *base) {
	assert(base != NULL);
	object *next = getNext(base);
	if (next != NULL) {
		swap(base, next);
	}
}

void swapPrev(object *base) {
	assert(base != NULL);
	object *prev = getPrev(base);
	if (prev != NULL) {
		swap(base, prev);
	}
}

void colorize(const char *col, char *str) {
	assert(str != NULL);
	char *temp = NULL;
	temp = growArray(temp, BUFSIZ, sizeof(char));

	snprintf(temp, BUFSIZ, "%s%s%s", col ? col : GRN, str, NOR);
	strlcpy(str, temp, BUFSIZ);

	temp = cleanPtr((char **)temp, NULL);
}

void isClean(void) {
	foreach (np) {
		assert(np != NULL);
		assert(np->name != NULL);
	}
}
