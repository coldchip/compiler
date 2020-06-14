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
	ScopeEntry *se = scope->list->entry;
	list_push(se->var, (void*)var);
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