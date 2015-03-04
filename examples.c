#include <dirent.h>
#include "list.h"

#define PATH "/Users/youri/Downloads"

int main(void) {
	DIR                 *dp;
	struct dirent       *ep;

	// initialize list
	unsigned int i = 0;
	TAILQ_INIT(&head);

	// get a file listing from PATH
	if ((dp = opendir(PATH)) == NULL) {
		perror("Failed to opendir");
		return -1;
	} else {
		while ((ep = readdir(dp)) != NULL) {
			if ((ep->d_name[0] != '.')) {
				//add(new(arc4random() % 100, ep->d_name));
				add(new(i++, ep->d_name));
			}
		}
		if (closedir(dp) == -1) {
			perror("Failed to close dir pointer");
			return -1;
		}
	}

	// swap last - 1 and last - 2 elements
#if 0
	swapNext(get(59));
	swapNext(get(59));
	swapPrev(get(59));
	struct entry *first = get(40);
	struct entry *second = get(2);
	swap(first, second);
	swapNext(get(39));
#endif

#if 0
	// replace next/prev entries
	struct entry e;
	e.name = growArray(e.name, BUFSIZ, sizeof(char));
	strlcpy(e.name, "youri", BUFSIZ);
	e.id = 99;
	setPrev(get(61), &e);
	//setNext(get(50), &e);
	//set(get(23), &e);
#endif


	// insert before / after
#if 0
	struct entry b, d;
	b.name = growArray(b.name, BUFSIZ, sizeof(char));
	d.name = growArray(d.name, BUFSIZ, sizeof(char));
	d.id = 11;
	b.id = 99;
	strlcpy(b.name, "youri", BUFSIZ);
	strlcpy(d.name, "kekek", BUFSIZ);
	addBefore(getLast(), &b);
	addAfter(getLast(), &d);
#endif

	// get first / last entry
#if 0
	printf("%s\n", toString(getFirst()));
	printf("%s\n", toString(getLast()));
#endif

	// get an entry like an array
#if 0
	struct entry *item = get(5);
	printf("5 > %s\n", toString(item));
	printf("%s\n", "renaming entry 5 name to \"arst\"");
	if (setName("arst", item) == -1) {
		perror("Failed to set name\n");
	} else {
		printf("%s\n", toString(item));
	}
#endif


	// convert linkedlist to array
#if 0
	char **array = NULL;
	array = toArray(array);
	unsigned int count = (unsigned int)getSize();
	dumpArray(array, count);
	cleanPtr(array, &count);
#endif

#if 0
	// bubble sort items
	//sort(cmpId);
	//sort(cmpName);
#endif

	// print contents
	foreach (np) {
		printf("%s\n", toString(np));
	}

	// free allocated memory for each entry
	// and remove them.
	freeList();

	// should be empty by now, still check
	if (!isEmpty()) {
		perror("ll not empty\n");
	}

	// free entry pointer
	free(np);
	np = NULL;

	return EXIT_SUCCESS;
}
