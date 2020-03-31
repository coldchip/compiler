#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

SymbolTable *symtable_init() {
	SymbolTable *st = malloc(sizeof(SymbolTable));
	TableEntry *first = malloc(sizeof(TableEntry));
	memset(st, 0, sizeof(SymbolTable));
	st->entry = first;
	st->start = first;
	return st;
}

void symtable_add(SymbolTable *st, char *name, int pointer) {

	TableEntry *prev = st->entry;

	TableEntry *next = malloc(sizeof(TableEntry));
	memset(next, 0, sizeof(TableEntry));

	char *name_malloc = malloc((sizeof(char) * strlen(name)) + 1);
	strcpy(name_malloc, name);

	next->name = name_malloc;
	next->pointer = pointer;

	prev->next = next;
	st->entry = next;
}

bool symtable_has(SymbolTable *st, char *name) {
	TableEntry *current = st->start->next;
	while(current != NULL) {
		if(strcmp(name, current->name) == 0) {
			return true;
		}
		current = current->next;
	}
	return false;
}

int symtable_ptr(SymbolTable *st, char *name) {
	TableEntry *current = st->start->next;
	while(current != NULL) {
		if(strcmp(name, current->name) == 0) {
			return current->pointer;
		}
		current = current->next;
	}
	return 0;
}

void symtable_free(SymbolTable *st) {
	TableEntry *current = st->start->next;
	while(current != NULL) {
		
		current = current->next;
	}
	return 0;
}