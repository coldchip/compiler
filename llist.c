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

List *list_clone(List *list) {
	List *new = list_init();

	ListEntry *current = list->start;
	while(current != NULL) {
		list_push(new, current->ptr);
		current = current->next;
	}
	return new;
}

void list_push(List *st, void *ptr) {

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

void *list_pop(List *st) {
	if(st->entry != NULL) {
		ListEntry *prev = st->entry->prev;
		void *ptr = st->entry->ptr;
		list_free_entry(st->entry);
		if(prev != NULL) {
			st->entry = prev;
			st->entry->next = NULL;
		} else {
			// Reset everything if popped to head
			st->start = NULL;
			st->entry = NULL;
		}
		return ptr;
	}
	return NULL;
}

void list_free_entry(ListEntry *le) {
	if(le != NULL) {
		free(le);
	}
}

void list_free(List *st) {
	ListEntry *current = st->start;
	while(current != NULL) {
		ListEntry *next = current->next;
		list_free_entry(current);
		current = next;
	}
	free(st);
}