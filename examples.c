#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

int main(void) {
	// list type test
	ArrayList *list;
	ArrayList *rlist;

	list = malloc(sizeof(*list));
	rlist = malloc(sizeof(*list));

	initList(list);
	initList(rlist);

	for (int i = 0; i < 10; i++) {
		printf("creating new element %u name \"%s\"\n", i, "test");
		list->obj = new(i, "test");
		add(list, list->obj);

		rlist->obj = new(100 - i, "revtest");
		add(rlist, rlist->obj);
	}

	list->obj = getLast(list);
	printf("last num : %u\n", list->obj->id);

	// XXX set() between lists does not work, use swap()
	//printf("removing 7 from list and adding it to rlist tail\n");
	//rlist->obj = set(rlist->obj, get(list, 7));
	//add(rlist, rlist->obj);
	//rm(list, get(list, 7));
	swap(get(list, 7), get(rlist, 97));
	rm(rlist, get(rlist, 7));

	printf("list\n");

	foreach(list) {
		toString(list->obj);
	}

	printf("rlist\n");

	foreach(rlist) {
		toString(rlist->obj);
	}

	cleanList(list);
	cleanList(rlist);

	if (!isEmpty(list) || !isEmpty(rlist)) {
		printf("list is supposed to be empty!\n");
	}

	assert(list->obj == NULL && rlist->obj == NULL);

	free(list);
	free(rlist);
	list = NULL;
	rlist = NULL;
	return 0;
}
