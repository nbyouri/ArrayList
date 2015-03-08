#include <dirent.h>
#include <assert.h>
#include "list.h"
#include "tools.h"

#define PATH "/Users/youri/Downloads"

int main(void) {
	// list type test
	ArrayList *list = NULL;
	list = malloc(sizeof(*list));

	initList(list);

	DIR                 *dp;
	struct dirent       *ep;

	unsigned int i = 0;

	// get a file listing from PATH
	if ((dp = opendir(PATH)) == NULL) {
		perror("Failed to opendir");
		return -1;
	} else {
		while ((ep = readdir(dp)) != NULL) {
			if ((ep->d_name[0] != '.')) {
				list->obj = new(i++, ep->d_name);
				add(list, list->obj);
			}
		}
		if (closedir(dp) == -1) {
			perror("Failed to close dir pointer");
			return -1;
		}
	}

	swap(get(list, 29), get(list, 2));

	// print contents
	foreach (list) {
		toString(list->obj);
	}

	// create new list and add an item from other list
	ArrayList *newlist;
	newlist = malloc(sizeof(*list));
	initList(newlist);

	newlist->obj = getFirst(list);
	add(newlist, newlist->obj);

	foreach (newlist) {
		toString(newlist->obj);
	}

	cleanList(newlist);

	// free list items
	cleanList(list);

	// should be empty by now, still check
	if (!isEmpty(list)) {
		perror("ll not empty\n");
	}

	printf("list->head: %p\nnewlist->head: %p\n", list->head, newlist->head);

	return isClean(list);
}
