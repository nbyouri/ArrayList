#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"

#define GRN "\033[22;32m"
#define NOR "\033[00;0m"

/*
 *
 * NEEDED TO USE:
 * - an entry structure containing at least
 *   entries, a set of pointers to previous
 *   and next elements.
 */
struct entry {
	unsigned int id;
	char *name;
	/* pointer to previous and next entries in the tail queue */
	TAILQ_ENTRY(entry) entries;
} *np;

// tail queue head
TAILQ_HEAD(tailhead, entry) head;

// Traverse the tail queue in forward direction
#define initList()	TAILQ_INIT(&head)
#define foreach(x)	TAILQ_FOREACH(x, &head, entries)
#define foreach_rev(x)	TAILQ_FOREACH_REVERSE(x, &head, tailhead, entries)

void toString(struct entry *);
void add(struct entry *);
void addBefore(struct entry *, struct entry *);
void addAfter(struct entry *, struct entry *);
void addOnTop(struct entry *);
void rm(struct entry *);
struct entry *new(unsigned int id, char *name);
struct entry *get(unsigned int);
struct entry *getFirst(void);
struct entry *getLast(void);
struct entry *getPrev(struct entry *);
struct entry *getNext(struct entry *);
struct entry *set(struct entry *, struct entry *);
void setPrev(struct entry *, struct entry *);
void setNext(struct entry *, struct entry *);
char *getName(struct entry *);
ssize_t setName(char *, struct entry *);
unsigned int getId(struct entry *);
size_t getSize(void);
int cmpName(const void *, const void *);
int cmpId(const void *, const void *);
void swap(struct entry *, struct entry *);
void swapPrev(struct entry *);
void swapNext(struct entry *);
void *toArray(char **);
bool isEmpty(void);
void sort(int (*)(const void *, const void *));
void cleanList(void);
void colorize(const char *, char *);
