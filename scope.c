#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

Scope *scope_init() {
	Scope *st = malloc(sizeof(Scope));
	st->entry = NULL;
	st->start = NULL;
	return st;
}

void scope_free(Scope *st) {
	ScopeEntry *current = st->start;
	while(current != NULL) {
		ScopeEntry *next = current->next;
		free(current);
		current = next;
	}
	free(st);
}