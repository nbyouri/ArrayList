#include "tools.h"

/*
 * Safely allocate memory for arrays
 */
void *growArray(void *ptr, size_t nelem, size_t size) {
	void *tempPtr;
	size_t totalSize = nelem * size;

	if (totalSize == 0) {
		printf("Trying to allocate 0\n");
		return NULL;
	}
	if (SIZE_T_MAX / nelem < size) {
		printf("Trying to allocate too much\n");
		return NULL;
	}
	// if the pointer is NULL or if we're trying
	// to grow a simple string, malloc first.
	if ((ptr == NULL) || (size == sizeof(char))) {
		tempPtr = malloc(totalSize);
	} else {
		tempPtr = realloc(ptr, totalSize);
	}
	if (tempPtr == NULL) {
		printf("Failed to reallocate %zu bytes.\n", totalSize);
	}

	return tempPtr;
}

/*
 * clean array of strings
 * can be used with char * as well,
 * just cast it. ex. cleanPtr((char **)str, NULL)
 */
void *cleanPtr(char **ptr, unsigned int *count) {
	if ((count != NULL) && (*count > 0)) {
		unsigned int i;
		for (i = 0; i < *count; i++) {
			free(ptr[i]);
			ptr[i] = NULL;
		}
		*count = 0;
	}
	free(ptr);
	ptr = NULL;
	return ptr;
}

