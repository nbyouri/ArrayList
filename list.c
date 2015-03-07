#include "list.h"
#include "tools.h"

#ifdef DEBUG
#include <assert.h>
#endif

void sort(int (*cmp)(const void *, const void *)) {
	unsigned int i = 0;
	size_t size = getSize();

	if (size == 0) {
		return;
	}

	for (i = 0; i < size; i++) {
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

void add(object *en) {
#ifdef DEBUG
	assert(en != NULL);
#endif
	TAILQ_INSERT_TAIL(&head, en, entries);
}

void addOnTop(object *en) {
#ifdef DEBUG
	assert(en != NULL);
#endif
	TAILQ_INSERT_HEAD(&head, en, entries);
}

void addBefore(object *base, object *new) {
#ifdef DEBUG
	assert(base != NULL && new != NULL);
#endif
	TAILQ_INSERT_BEFORE(base, new, entries);
}

void addAfter(object *base, object *new) {
#ifdef DEBUG
	assert(base != NULL && new != NULL);
#endif
	TAILQ_INSERT_AFTER(&head, base, new, entries);
}

void rm(object *en) {
#ifdef DEBUG
	assert(en != NULL);
#endif
	TAILQ_REMOVE(&head, en, entries);
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
#ifdef DEBUG
	assert(en != NULL);
#endif
	object *prev;
	prev =  TAILQ_PREV(en, tailhead, entries);
	return prev;
}

object *getNext(object *en) {
#ifdef DEBUG
	assert(en != NULL);
#endif
	object *next;
	next = TAILQ_NEXT(en, entries);
	return next;
}

void setPrev(object *base, object *en) {
#ifdef DEBUG
	assert(base != NULL && en != NULL);
#endif
	object *prev = getPrev(base);
	if (prev != NULL) {
		set(prev, en);
	}
}

void setNext(object *base, object *en) {
#ifdef DEBUG
	assert(base != NULL && en != NULL);
#endif
	object *next = getNext(base);
	if (next != NULL) {
		set(next, en);
	}
}

unsigned int getId(object *en) {
#ifdef DEBUG
	assert(en != NULL);
#endif
	return en->id;
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
#ifdef DEBUG
	assert(first != NULL && second != NULL);
#endif
	if (first->id != second->id) {
		object *temp = NULL;
		temp = set(temp, first);

		first = set(first, second);
		second = set(second, temp);

		temp = cleanPtr((char **)temp, NULL);
	}
}

void swapNext(object *base) {
#ifdef DEBUG
	assert(base != NULL);
#endif
	object *next = getNext(base);
	if (next != NULL) {
		swap(base, next);
	}
}

void swapPrev(object *base) {
#ifdef DEBUG
	assert(base != NULL);
#endif
	object *prev = getPrev(base);
	if (prev != NULL) {
		swap(base, prev);
	}
}

