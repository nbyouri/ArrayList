#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

#define PATH "/Users/youri/list"
void print_head(struct list_head *head)
{
  printf("-- HEAD\n");
  printf("%p: head\n", head);
  printf("%p: head->tqh_first      = %p\n", &(head->tqh_first), head->tqh_first);
  printf("%p: head->tqh_last       = %p\n", &(head->tqh_last), head->tqh_last);
}
void print_items(struct list_head *head)
{
  struct entry *item;

  /* headが参照するリストをentriesメンバを使ってループする。要素はitemに入れる。 */
  TAILQ_FOREACH(item, head, entries) {
    printf("-- TAILQ_FOREACH %d\n", item->id);

    /* itemのアドレス */
    printf("%p: item\n", item);

    printf("%p: item->id               = %d\n", &(item->id), item->id);
    printf("%p: item->entries.tqe_next = %p\n", &(item->entries.tqe_next), item->entries.tqe_next);
    printf("%p: item->entries.tqe_prev = %p\n", &(item->entries.tqe_prev), item->entries.tqe_prev);

    /*
      以下、TAILQ_PREV がどういう仕組みなのか調べるための出力。

      TAILQ_PREVは、指定した要素の1つ前の要素を得るためのマクロ。
      (*(((struct list_head *)(item->entries.tqe_prev))->tqh_last))
      のように展開される。

      item->entries.tqe_prev が指す先を
      list_headと見なし(struct list_head * にキャスト)、
      構造体の2番目のメンバ(tqh_last = tqe_prev)が指す先に書かれた
      アドレス(tqe_next or tqh_last)が「1つ前の要素」を指している。
      「2歩戻って1歩進む」

      headまでいくと、tqh_lastによってNULLが返る。
      ループが正順でも逆順でも、最後の要素のtqe_nextが終了条件として機能している。
    */
    struct entry **prev = item->entries.tqe_prev;
    struct list_head *prev_head = (struct list_head *)prev;
    printf("             prev_head            = %p\n", prev_head);
    printf("             prev_head->tqh_last  = %p\n", prev_head->tqh_last);
    printf("            *prev_head->tqh_last  = %p\n", *prev_head->tqh_last);
  }
}
int main(void) {
	// list type test
	ArrayList *list = NULL;
	list = malloc(sizeof(*list));

	initList(list);
	//XXX debug give head names
	list->head->head_name = malloc(BUFSIZ);
	strlcpy(list->head->head_name, "headone", BUFSIZ);
	list->obj = malloc(sizeof(object));

	DIR                 *dp;
	struct dirent       *ep;

	unsigned int i = 0;

//	// get a file listing from PATH
//	if ((dp = opendir(PATH)) == NULL) {
//		perror("Failed to opendir");
//		return -1;
//	} else {
//		while ((ep = readdir(dp)) != NULL) {
//			if ((ep->d_name[0] != '.')) {
//				add(list, new(i++, ep->d_name));
//			}
//		}
//		if (closedir(dp) == -1) {
//			perror("Failed to close dir pointer");
//			return -1;
//		}
//	}

	//swap(get(list, 29), get(list, 2));
	add(list, new(3, ""));
	add(list, new(43, ""));
	//add(list, new(2, "test2"));
	//add(list, new(3, "test3"));
	//struct entry new;
	//new.id = 3;
	//TAILQ_INSERT_TAIL(list->head, &new, entries);
	//struct entry other;
	//other.id = 43;
	//TAILQ_INSERT_TAIL(list->head, &other, entries);

	// print contents
	//foreach (list) {
	//	toString(list->obj);
	//	printf("object %u {\n\ttemp\t\t%p\n\ttqe_next\t%p\n\ttqe_prev\t%p"
	//	    "\n\t*tqe_prev\t%p\n};\n", getId(list->obj), list->obj,
	//	    list->obj->entries.tqe_next, list->obj->entries.tqe_prev,
	//	    *(list->obj->entries.tqe_prev));
	//}
	//printf("head %s {\n\ttqh_first\t%p\n\ttqh_last\t%p\n\t*tqh_last\t%p\n};\n",
	//    list->head->head_name, list->head->tqh_first, list->head->tqh_last, *(list->head->tqh_last));
	//puts("tweaking head->tqh_last");

	////struct entry *ptr = get(list, 12);
	////list->head->tqh_last = ptr->entries.tqe_next;
	////*(list->head->tqh_last) = NULL;
	//printf("head %s {\n\ttqh_first\t%p\n\ttqh_last\t%p\n\t*tqh_last\t%p\n};\n",
	//    list->head->head_name, list->head->tqh_first, list->head->tqh_last, *(list->head->tqh_last));

	////struct  entry *temp = malloc(sizeof(struct entry));
	////temp = TAILQ_LAST(list->head, list_head);
	////printf("last = %u (%p)\n", temp->id, temp);
	print_items(list->head);
	print_head(list->head);
	//list->obj = TAILQ_LAST(list->head, list_head);
	object **ptr = list->head->tqh_last;
	printf("last pointer : %p\n", ptr);

	//ArrayList *newlist;
	//newlist = malloc(sizeof(*list));
	//initList(newlist);

	//newlist->obj = get(list, 12);
	//add(newlist, newlist->obj);

	//add(newlist, getLast(list));
	//add(newlist, getFirst(list));
	//add(newlist, get(list, 4));
	//printf("\n\n\nlist2\n");
	//foreach (newlist) {
	//	toString(newlist->obj);
	//}

	// should be empty by now, still check
#if 0
	printf("trace : lastfile : %s\nlastline : %d\nprevfile : %s\n"
	    "prevline : %d\n", list->head->trace.lastfile,
	    list->head->trace.lastline,
	    list->head->trace.prevfile,
	    list->head->trace.prevline);
	if (isEmpty(list)) { //|| isEmpty(newlist)) {
		perror("ll empty\n");
		return -1;
	} else {

		//cleanList(newlist);

		// free list items
		//cleanList(list);

		printf("lists cleaned, you can't do anything with them now\n");

	}
#endif
	return 0;
	}
