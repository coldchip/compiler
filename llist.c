#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

List *list_init() {
	List *st = malloc(sizeof(List));
	st->entry = NULL;
	st->start = NULL;
	return st;
}

List *list_clone(List *st) {
	List *st_new = list_init();

	ListEntry *current = st->start;
	while(current != NULL) {
		list_add(st_new, current->ptr);
		current = current->next;
	}
	return st_new;
}
 
void list_add(List *st, void *ptr) {

	ListEntry *next = malloc(sizeof(ListEntry));
	memset(next, 0, sizeof(ListEntry));

	next->ptr = ptr;

	if(st->entry == NULL) {
		st->start = next;
	} else {
		st->entry->next = next;
	}
	st->entry = next;
}

void list_free(List *st) {
	ListEntry *current = st->start;
	while(current != NULL) {
		ListEntry *next = current->next;
		free(current);
		current = next;
	}
	free(st);
}