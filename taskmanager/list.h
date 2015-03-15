#include <sys/types.h>
#include <limits.h>

#include "queue.h"

/* pointers is a tailq entry */
#define pointers	TAILQ_ENTRY(entry)

/* list head */
TAILQ_HEAD(list_head, entry);

/* user defined entry structure */
#include "entry.h"

/* initialize the list */
#define initList(x)	do {				\
	x->head = malloc(sizeof(struct list_head));	\
	TAILQ_INIT(x->head);				\
} while (0)

/* Traverse the tail queue in forward direction */
#define foreach(x)	TAILQ_FOREACH(x->obj, x->head, entries)

/* Traverse the tail queue in forward direction */
#define foreach_rev(x)	TAILQ_FOREACH_REVERSE(x->obj, x->head, list_head, entries)

void toString(object *);
void add(ArrayList *, object *);
void addOnTop(ArrayList *, object *);
void rm(ArrayList *, object *);
void swapPrev(ArrayList *, object *);
void swapNext(ArrayList *, object *);

void addBefore(ArrayList *, object *, object *);
void addAfter(ArrayList *, object *, object *);
void setPrev(ArrayList *,  object *, object *);
void setNext(ArrayList *,  object *, object *);
void swap(object *, object *);

object  *get(ArrayList *, unsigned int);

object  *getFirst(ArrayList *);
object  *getLast(ArrayList *);

object  *getPrev(ArrayList *, object *);
object  *getNext(ArrayList *, object *);

unsigned int getId(object *);
unsigned int getSize(ArrayList *);

void *toArray(ArrayList *);

int isEmpty(ArrayList *);

void cleanList(ArrayList *);

void sort(ArrayList *, int (*)(const void *, const void *));
