#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

int main(void) {
	// list type test
	ArrayList *list = NULL;
	list = malloc(sizeof(*list));

	initList(list);

	for (int i = 0; i < 10; i++) {
		printf("creating new element %u name \"%s\"\n", i, "test");
		list->obj = new(i, "test");
		add(list, list->obj);
	}

	list->obj = getLast(list);
	printf("last num : %u\n", list->obj->id);

	printf("removing 7\n");
	rm(list, get(list, 7));

	foreach(list) {
		toString(list->obj);
	}

	cleanList(list);

	if (!isEmpty(list)) {
		printf("list is supposed to be empty!\n");
	}

	free(list);
	list = NULL;
	return 0;
}
