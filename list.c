#include "list.h"
#include "tools.h"

#ifdef DEBUG
#include <assert.h>
#endif

void sort(ArrayList *list, int (*cmp)(const void *, const void *)) {
	unsigned int i = 0;
	unsigned int size = getSize(list);

	if (size == 0) {
		return;
	}

	for (i = 0; i < size; i++) {
		foreach(list) {
			object *next = getNext(list, list->obj);
			if (next != NULL) {
				if (cmp(list->obj, next) > 0) {
					swap(list->obj, next);
				}
			}
		}
	}
}

void add(ArrayList *list, object *obj) {
#ifdef DEBUG
	assert(obj != NULL);
#endif
	TAILQ_INSERT_TAIL(list->head, obj, entries);
}

void addOnTop(ArrayList *list, object *obj) {
#ifdef DEBUG
	assert(obj != NULL);
#endif
	TAILQ_INSERT_HEAD(list->head, obj, entries);
}

void addBefore(ArrayList *list, object *base, object *new) {
#ifdef DEBUG
	assert(base != NULL && new != NULL);
#endif
	TAILQ_INSERT_BEFORE(base, new, entries);
}

void addAfter(ArrayList *list, object *base, object *new) {
#ifdef DEBUG
	assert(base != NULL && new != NULL);
#endif
	TAILQ_INSERT_AFTER(list->head, base, new, entries);
}

void rm(ArrayList *list, object *obj) {
#ifdef DEBUG
	assert(obj != NULL);
#endif
	TAILQ_REMOVE(list->head, obj, entries);
}

object *get(ArrayList *list, unsigned int i) {
	foreach (list) {
		if (list->obj->id == i) {
			return list->obj;
		}
	}
	return NULL;
}

object *getFirst(ArrayList *list) {
	return TAILQ_FIRST(list->head);
}

object *getLast(ArrayList *list) {
	return TAILQ_LAST(list->head, list_head);
}

object *getPrev(ArrayList *list, object *obj) {
#ifdef DEBUG
	assert(obj != NULL);
#endif
	object *prev;
	prev =  TAILQ_PREV(obj, list_head, entries);
	return prev;
}

object *getNext(ArrayList *list, object *obj) {
#ifdef DEBUG
	assert(obj != NULL);
#endif
	object *next;
	next = TAILQ_NEXT(obj, entries);
	return next;
}

void setPrev(ArrayList *list, object *base, object *obj) {
#ifdef DEBUG
	assert(base != NULL && obj != NULL);
#endif
	object *prev = getPrev(list, base);
	if (prev != NULL) {
		set(prev, obj);
	}
}

void setNext(ArrayList *list, object *base, object *en) {
#ifdef DEBUG
	assert(base != NULL && en != NULL);
#endif
	object *next = getNext(list, base);
	if (next != NULL) {
		set(next, en);
	}
}

unsigned int getId(object *obj) {
#ifdef DEBUG
	assert(obj != NULL);
#endif
	return obj->id;
}

unsigned int getSize(ArrayList *list) {
	unsigned int size = 0;
	foreach (list) {
		size++;
	}
	return size;
}

int isEmpty(ArrayList *list) {
	return TAILQ_EMPTY(list->head);
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

		free(temp);
		temp = NULL;
	}
}

void swapNext(ArrayList *list, object *base) {
#ifdef DEBUG
	assert(base != NULL);
#endif
	object *next = getNext(list, base);
	if (next != NULL) {
		swap(base, next);
	}
}

void swapPrev(ArrayList *list, object *base) {
#ifdef DEBUG
	assert(base != NULL);
#endif
	object *prev = getPrev(list, base);
	if (prev != NULL) {
		swap(base, prev);
	}
}

