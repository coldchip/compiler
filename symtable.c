#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

SymbolTable *symtable_init() {
	SymbolTable *st = malloc(sizeof(SymbolTable));
	st->entry = NULL;
	st->start = NULL;
	return st;
}

SymbolTable *symtable_clone(SymbolTable *st) {
	SymbolTable *st_new = symtable_init();

	TableEntry *current = st->start;
	while(current != NULL) {
		symtable_add(st_new, current->name, current->pointer);
		current = current->next;
	}
	return st_new;
}

void symtable_add(SymbolTable *st, char *name, int pointer) {

	TableEntry *next = malloc(sizeof(TableEntry));
	memset(next, 0, sizeof(TableEntry));

	char *name_malloc = malloc((sizeof(char) * strlen(name)) + 1);
	strcpy(name_malloc, name);

	next->name = name_malloc;
	next->pointer = pointer;

	if(st->entry == NULL) {
		st->start = next;
	} else {
		st->entry->next = next;
	}
	st->entry = next;
}

bool symtable_has(SymbolTable *st, char *name) {
	TableEntry *current = st->start;
	while(current != NULL) {
		if(strcmp(name, current->name) == 0) {
			return true;
		}
		current = current->next;
	}
	return false;
}

int symtable_ptr(SymbolTable *st, char *name) {
	TableEntry *current = st->start;
	while(current != NULL) {
		if(strcmp(name, current->name) == 0) {
			return current->pointer;
		}
		current = current->next;
	}
	return 0;
}

void symtable_free(SymbolTable *st) {
	TableEntry *current = st->start;
	while(current != NULL) {
		TableEntry *next = current->next;
		free(current->name);
		free(current);
		current = next;
	}
	free(st);
}