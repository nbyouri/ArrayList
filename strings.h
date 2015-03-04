#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void *cleanPtr(char **, unsigned int *);
char **stringToArray(char *, char *);
char *readinput(const char *);
void ReadFD(int *);
void *growArray(void *, size_t, size_t);
void dumpArray(char **, size_t);
