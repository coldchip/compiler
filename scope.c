#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

Scope *scope_init() {
	Scope *st = malloc(sizeof(Scope));
	st->var = list_init();
	st->offset = 0;
	return st;
}

Scope *scope_clone(Scope *scope) {
	Scope *new = scope_init();
	list_free(new->var);
	new->var = list_clone(scope->var);
	return new;
}

void scope_add_var(Scope *scope, const char *var) {
	List *varlist = scope->var;
	list_push(varlist, (void*)var);
}

bool scope_has_var(Scope *scope, const char *var) {
	List *varlist = scope->var;
	ListEntry *entry = varlist->start;
	while(entry != NULL) {
		if(strcmp(entry->ptr, var) == 0) {
			return true;
		}
		entry = entry->next;
	}
	return false;
}

void scope_free(Scope *scope) {
	if(scope != NULL) {
		list_free(scope->var);
		free(scope);
	}
}