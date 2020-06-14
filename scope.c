#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

// fifo data structure

Scope *scope_init() {
	Scope *st = malloc(sizeof(Scope));
	st->list = list_init();
	return st;
}

void scope_add_var(Scope *scope, const char *var) {
	ScopeEntry *se = scope_get_current(scope);
	list_add(se->var, var);
}

void scope_push(Scope *scope) {
	ScopeEntry *se = malloc(sizeof(ScopeEntry));
	se->var = list_init();
	list_add(scope->list, se);
}

void scope_pop(Scope *scope) {
	ListEntry *entry = list_get_entry(scope->list);
	while(1) {
		if(entry->next == NULL) {
			if(entry->prev) {
				entry->prev->next = NULL;
			} else {
				scope->list->start = NULL;
			}
			scope_entry_free(entry->ptr);
			list_free_entry(entry);
			break;
		}
		entry = entry->next;
	}
}

ScopeEntry *scope_get_current(Scope *scope) {
	ListEntry *entry = list_get_entry(scope->list);
	while(entry != NULL) {
		ListEntry *next = entry->next;
		if(next == NULL) {
			return entry;
		}
		entry = next;
	}
}

void scope_entry_free(ScopeEntry *se) {
	list_free(se->var);
	free(se);
}

void scope_free(Scope *scope) {
	list_free(scope->list);
	free(scope);
}