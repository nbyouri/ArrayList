#include "list.h"
#include "strings.h"

void sort(int (*cmp)(const void *, const void *)) {
	unsigned int i = 0;
	for (i = 0; i < getSize(); i++) {
		foreach(np) {
			struct entry *next = getNext(np);
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

// XXX memory leak
char *toString(struct entry *en) {
	char *string = NULL;
	string = growArray(string, BUFSIZ, sizeof(char));
	snprintf(string, BUFSIZ, "%02u -> %s", en->id, en->name);
	return string;
}

void cleanList() {
	foreach (np) {
		np->name = cleanPtr((char **)np->name, NULL);
		assert(np->name == NULL);
		rm(np);
	}
}

void add(struct entry *en) {
	assert(en != NULL);
	TAILQ_INSERT_TAIL(&head, en, entries);
}

void addOnTop(struct entry *en) {
	assert(en != NULL);
	TAILQ_INSERT_HEAD(&head, en, entries);
}

void addBefore(struct entry *base, struct entry *new) {
	assert(base != NULL && new != NULL);
	TAILQ_INSERT_BEFORE(base, new, entries);
}

void addAfter(struct entry *base, struct entry *new) {
	assert(base != NULL && new != NULL);
	TAILQ_INSERT_AFTER(&head, base, new, entries);
}

void rm(struct entry *en) {
	assert(en != NULL);
	TAILQ_REMOVE(&head, en, entries);
}

struct entry *new(unsigned int id, char *name) {
	struct entry *en = NULL;
	en = growArray(en, 1, sizeof(*en));
	en->id = id;
	en->name = growArray(en->name, BUFSIZ, sizeof(*(en->name)));
	if (name != NULL) {
		strlcpy(en->name, name, BUFSIZ);
	}
	return en;
}

struct entry *get(unsigned int i) {
	foreach (np) {
		if (np->id == i) {
			return np;
		}
	}
	return NULL;
}

struct entry *getFirst(void) {
	return TAILQ_FIRST(&head);
}

struct entry *getLast(void) {
	return TAILQ_LAST(&head, tailhead);
}

struct entry *getPrev(struct entry *en) {
	assert(en != NULL);
	struct entry *prev;
	prev =  TAILQ_PREV(en, tailhead, entries);
	return prev;
}

struct entry *getNext(struct entry *en) {
	assert(en != NULL);
	struct entry *next;
	next = TAILQ_NEXT(en, entries);
	return next;
}

struct entry *set(struct entry *base, struct entry *en) {
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

void setPrev(struct entry *base, struct entry *en) {
	assert(base != NULL && en != NULL);
	struct entry *prev = getPrev(base);
	if (prev != NULL) {
		set(prev, en);
	}
}

void setNext(struct entry *base, struct entry *en) {
	assert(base != NULL && en != NULL);
	struct entry *next = getNext(base);
	if (next != NULL) {
		set(next, en);
	}
}

char *getName(struct entry *en) {
	return (en->name);
}

unsigned int getId(struct entry *en) {
	assert(en != NULL);
	return en->id;
}

int cmpId(const void *a, const void *b) {
	struct entry *en1 = (struct entry *)a;
	struct entry *en2 = (struct entry *)b;
	assert(en1 != NULL && en2 != NULL);
	return ((int)en2->id - (int)en1->id);
}

ssize_t setName(char *name, struct entry *en) {
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

bool isEmpty(void) {
	return TAILQ_EMPTY(&head);
}

int cmpName(const void *a, const void *b) {
	struct entry *en1 = (struct entry *)a;
	struct entry *en2 = (struct entry *)b;
	assert(en1 != NULL && en2 != NULL);
	return (strcasecmp(en1->name, en2->name));
}

void swap(struct entry *first, struct entry *second) {
	assert(first != NULL && second != NULL);
	if (first->id != second->id) {
		struct entry *temp = NULL;
		temp = set(temp, first);

		first = set(first, second);
		second = set(second, temp);

		free(temp);
		temp = NULL;
	}
}

void swapNext(struct entry *base) {
	assert(base != NULL);
	struct entry *next = getNext(base);
	if (next != NULL) {
		swap(base, next);
	}
}

void swapPrev(struct entry *base) {
	assert(base != NULL);
	struct entry *prev = getPrev(base);
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

	free(temp);
	temp = NULL;
}

