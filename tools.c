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

/* dumps the content of a string array with given size
 * should work with char * with casting to char ** and
 * setting the size to 0.
 */
void dumpArray(char **ptr, size_t size) {
	if (ptr == NULL) {
		printf("Can't dump NULL pointer\n");
		return;
	}

	unsigned int i = 0;

	// deal with arrays of characters
	if (size == 0) {
		if (*ptr == NULL) {
			printf("NULL string.\n");
		} else {
			size_t strSize = strlen((char *)ptr);
			if (strSize > 0) {
				printf("%s[%zu] = {\n", (char *)ptr, strSize);
				for (i = 0; i < strSize; i++) {
					printf("\t%c, // (id. %u)\n",
					    *((char *)ptr + i), i);
				}
			} else {
				printf("Can't dump empty array\n");
			}
		}
		printf("};\n");
		return;
	}

	// deal with arrays of arrays of characters
	printf("args[%zu] = {\n", size + 1);

	// loop through array elements
	for (i = 0; i <= size; i++) {
		printf("\t");
		if (ptr[i] == NULL) {
			printf("NULL,\n");
		} else {
			printf("%s, // (%zu)\n", ptr[i], strlen(ptr[i]));
		}
	}
	printf("};\n");
}
