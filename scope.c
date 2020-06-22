#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

void scope_add_var(Scope *scope, const char *var, uint64_t offset) {
	Var *entry = malloc(sizeof(Var));
	entry->var = (char*)var;
	entry->offset = offset;

	List *varlist = scope->var;
	list_push(varlist, (void*)entry);
}

uint64_t scope_get_offset(Scope *scope, const char *var) {
	List *varlist = scope->var;
	ListEntry *entry = varlist->start;
	while(entry != NULL) {
		Var *var_entry = (Var*)(entry->ptr);
		if(strcmp(var_entry->var, var) == 0) {
			return var_entry->offset;
		}
		entry = entry->next;
	}
	return 0;
}

bool scope_has_var(Scope *scope, const char *var) {
	List *varlist = scope->var;
	ListEntry *entry = varlist->start;
	while(entry != NULL) {
		Var *var_entry = (Var*)(entry->ptr);
		if(strcmp(var_entry->var, var) == 0) {
			return true;
		}
		entry = entry->next;
	}
	return false;
}

void scope_free(Scope *scope) {
	if(scope != NULL) {
		List *varlist = scope->var;
		ListEntry *entry = varlist->start;
		while(entry != NULL) {
			Var *var_entry = (Var*)entry->ptr;
			free(var_entry);
			entry = entry->next;
		}
		list_free(scope->var);
		free(scope);
	}
}