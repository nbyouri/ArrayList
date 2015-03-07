#include <dirent.h>
#include "list.h"
#include "tools.h"

#define PATH "/Users/youri/Downloads"

int main(void) {
	DIR                 *dp;
	struct dirent       *ep;

	// initialize list
	initList();
	unsigned int i = 0;

	// get a file listing from PATH
	if ((dp = opendir(PATH)) == NULL) {
		perror("Failed to opendir");
		return -1;
	} else {
		while ((ep = readdir(dp)) != NULL) {
			if ((ep->d_name[0] != '.')) {
				np = new(i++, ep->d_name);
				add(np);
			}
		}
		if (closedir(dp) == -1) {
			perror("Failed to close dir pointer");
			return -1;
		}
	}

#if 0
	// swap last - 1 and last - 2 elements
	swapNext(get(9));
	swapNext(get(9));
	swapPrev(get(9));
#endif
	swap(get(29), get(2));
	swapNext(get(9));

#if 0
	// replace next/prev entries
	object e;
	e.name = growArray(e.name, BUFSIZ, sizeof(char));
	strlcpy(e.name, "youri", BUFSIZ);
	e.id = 99;
	setPrev(get(1), &e);
	//setNext(get(50), &e);
	//set(get(23), &e);


	// insert before / after
	object b, d;
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
	toString(getFirst());
	toString(getLast());
#endif

#if 0
	// get an entry like an array
	object *item = get(5);
	toString(get(5));
	printf("%s\n", "renaming entry 5 name to \"arst\"");
	if (setName("arst", item) == -1) {
		perror("Failed to set name\n");
	} else {
		toString(item))
	}
#endif


	// convert linkedlist to array
	char **array = NULL;
	array = toArray(array);
	unsigned int count = (unsigned int)getSize();
	count--;
	dumpArray(array, count);
	cleanPtr(array, &count);

	// bubble sort items
#if 0
	sort(cmpId);
	sort(cmpName);
#endif

	// print contents
	foreach (np) {
		toString(np);
	}

	// free list items
	cleanList();

	// should be empty by now, still check
	if (!isEmpty()) {
		perror("ll not empty\n");
	}

	// free entry pointer
	np = cleanPtr((char **)np, NULL);

	isClean();

	return 0;
}
