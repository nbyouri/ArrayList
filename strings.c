#include "strings.h"

// read string from user
char *readinput(const char *prompt) {
	char line[BUFSIZ];

	printf("%s", prompt);

	fgets(line, sizeof(line), stdin);
	if (line != NULL) {
		// don't do anything if user just presses return
		if (line[0] == '\n') {
			return NULL;
		}
		// shave off the \n at the end
		line[strlen(line) - 1] = '\0';
		// copy the string safely
		char *string = NULL;
		string = growArray(string, BUFSIZ, sizeof(char));
		strlcpy(string, line, BUFSIZ);
		return string;
	} else {
		return NULL;
	}
}

// procedure to read lines from file descriptor
void ReadFD(int *fd) {
	char buf[BUFSIZ];
	FILE *file;
	file = fdopen(*fd, "r");
	if (file == NULL) {
		perror("Failed to fdopen\n");
		exit(EXIT_FAILURE);
	} else {
		while (!feof(file) &&
		    !ferror(file) &&
		    fgets(buf, sizeof(buf), file) != NULL) {
			printf("%s", buf);
		}
		if (fclose(file) == EOF) {
			perror("Failed to fclose\n");
		}
	}
}

// grow array of one element of given size
// copy of xrealloc from openssh
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

// clean array of strings
// can be used with char * as well,
// just cast it. ex. cleanPtr((char **)str, NULL)
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

// convert string to array of strings separated by delimiters
char **stringToArray(char *string, char *delims) {
	char    *token;
	char    **array = NULL;
	unsigned int    i = 0;

	// separate the string into tokens based on the delimiters
	// and store these tokens in an array. token is set to NULL
	// when strsep is done.
	while ((token = strsep(&string, delims)) != NULL) {
		// grow array at index + 1
		array = growArray(array, i + 1, sizeof(char *));
		if (array == NULL) {
			printf("Failed to reallocate array at index %u\n", i);
			array = cleanPtr(array, &i);
			return NULL;
		} else {
			// allocate string in array
			array[i] = growArray(array[i], BUFSIZ, sizeof(char));
			if (array[i] == NULL) {
				printf("Failed to malloc array[%u]\n", i);
				array = cleanPtr(array, &i);
				return NULL;
			} else {
				// don't copy an empty string
				if (strlen(token) > 0) {
					strlcpy(array[i], token, BUFSIZ);
					i++;
				}
			}
		}
	}

	// make sure the array of string end with NULL
	array[i] = NULL;

	// debug array content
	dumpArray(array, i);

	return array;
}

// dumps the content of a string array with given size
// should work with char * with casting to char ** and
// setting the size to 0.
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
