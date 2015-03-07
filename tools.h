#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void *cleanArray(char **, unsigned int *);
int growArray(void *, size_t, size_t);
void dumpArray(char **, size_t);
