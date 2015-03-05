#include <sys/types.h>

#include "queue.h"

/* pointers is a tailq entry */
#define pointers	TAILQ_ENTRY(entry)

/* user defined entry structure */
#include "entry.h"

/* tail queue head */
TAILQ_HEAD(tailhead, entry) head;

/* initialize the list */
#define initList()	TAILQ_INIT(&head)

/* Traverse the tail queue in forward direction */
#define foreach(x)	TAILQ_FOREACH(x, &head, entries)

/* Traverse the tail queue in forward direction */
#define foreach_rev(x)	TAILQ_FOREACH_REVERSE(x, &head, tailhead, entries)

void toString(object *);

void add(object *);
void addBefore(object *, object *);
void addAfter(object *, object *);
void addOnTop(object *);

void rm(object *);

object *get(unsigned int);
object *getFirst(void);
object *getLast(void);
object *getPrev(object *);
object *getNext(object *);
object *set(object *, object *);

void setPrev(object *, object *);
void setNext(object *, object *);

unsigned int getId(object *);
size_t getSize(void);

void swap(object *, object *);
void swapPrev(object *);
void swapNext(object *);

void *toArray(char **);

int isEmpty(void);
void isClean(void);
void cleanList(void);

void sort(int (*)(const void *, const void *));
