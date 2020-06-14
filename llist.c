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
		next->prev = NULL;
	} else {
		st->entry->next = next;
		next->prev = st->entry;
	}
	st->entry = next;
}

ListEntry *list_get_entry(List *st) {
	return st->start;
}

void list_free_entry(ListEntry *le) {
	free(le);
}

void list_free(List *st) {
	ListEntry *current = list_get_entry(st);
	while(current != NULL) {
		ListEntry *next = current->next;
		list_free_entry(current);
		current = next;
	}
	free(st);
}