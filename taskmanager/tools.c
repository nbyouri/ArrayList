#include "tools.h"
#include <err.h>

/*
 * Join a number of strings to form one long string
 * separated by the optional separator.
 *
 * param: array : a NULL terminated array of strings to join
 *        separ : a separator or NULL.
 */
char *strjoin(const char *separ, char **array) {
        /* get array size */
        size_t siz = 0;

        /* get string count */
        while (array[siz++] != NULL);

        /* allocate string */
        char *str = NULL;
        str = malloc(siz * BUFSIZ);
        if (str == NULL)
                err(1, "malloc failed :");

        /* copy first string entry */
        strlcpy(str, array[0], BUFSIZ);

        /* concatenate the rest */
        for (size_t i = 1; array[i] != NULL && strlen(array[i]) > 0; i++) {
                if (separ != NULL) {
                        strlcat(str, separ, BUFSIZ);
                }

                strlcat(str, array[i], BUFSIZ);
        }

        str[strlen(str)] = '\0';

        return str;

}

/*
 * Safely allocate memory for arrays
 */
#ifndef __NetBSD__
int growArray(void *ptr, size_t num, size_t size) {
        int saved_errno, result;
        void *optr;
        void *nptr;

        memcpy(&optr, ptr, sizeof(ptr));
        saved_errno = errno;
        if (num == 0 || size == 0) {
                free(optr);
                nptr = NULL;
                memcpy(ptr, &nptr, sizeof(ptr));
                errno = saved_errno;
                return 0;
        }
        if ((num >= 65535 || size >= 65535) && num > SIZE_MAX / size) {
                return EOVERFLOW;
        }

        nptr = realloc(optr, num * size);
        if (nptr == NULL) {
                result = errno;
        } else {
                result = 0;
                memcpy(ptr, &nptr, sizeof(ptr));
        }
        errno = saved_errno;
        return result;
}
#endif

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
        if (size == -1) {
                for (i = 0; ptr[i] != NULL; i++) {
                        printf("\t");
                        printf("%s\n", ptr[i]);
                }
        } else {
                for (i = 0; i <= size; i++) {
                        printf("\t");
                        if (ptr[i] == NULL) {
                                printf("NULL,\n");
                        } else {
                                printf("%s, // (%zu)\n", ptr[i], strlen(ptr[i]));
                        }
                }
        }
        printf("};\n");
}
