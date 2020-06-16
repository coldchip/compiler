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
	List *varlist = ((ScopeEntry*)scope->list->entry->ptr)->var;
	list_push(varlist, (void*)var);
}

bool scope_has_var(Scope *scope, const char *var) {
	List *varlist = ((ScopeEntry*)scope->list->entry->ptr)->var;
	ListEntry *entry = varlist->start;
	while(entry != NULL) {
		if(strcmp(entry->ptr, var) == 0) {
			return true;
		}
		entry = entry->next;
	}
	return false;
}

void scope_push(Scope *scope) {
	ScopeEntry *entry = malloc(sizeof(ScopeEntry));
	entry->var = list_init();
	list_push(scope->list, entry);
}

void scope_pop(Scope *scope) {
	ScopeEntry *entry = list_pop(scope->list);
	scope_entry_free(entry);
}

void scope_entry_free(ScopeEntry *entry) {
	if(entry != NULL) {
		list_free(entry->var);
		free(entry);
	}
}

void scope_free(Scope *scope) {
	if(scope != NULL) {
		list_free(scope->list);
		free(scope);
	}
}