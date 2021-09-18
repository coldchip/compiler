#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "varlist.h"

int var_get_total_allocated_size(List *var) {
	if(list_size(var) > 0) {
		VarScope *vs = (VarScope*)list_back(var);
		return vs->offset + vs->size;
	}
	return 0;
}

VarScope *var_get(List *var, char *name) {
	for(ListNode *i = list_begin(var); i != list_end(var); i = list_next(i)) {
		VarScope *vs = (VarScope*)i;
		if(strcmp(name, vs->name) == 0) {
			return vs;
		}
	}
	return NULL;
}

VarScope *var_insert(List *var, char *name, int size) {
	VarScope *vs = malloc(sizeof(VarScope));
	vs->name     = name;
	vs->size     = size;
	vs->offset   = var_get_total_allocated_size(var);

	list_insert(list_end(var), vs);

	return vs;
}

VarScope *var_insert_func(List *var, char *name, int size) {
	VarScope *vs = malloc(sizeof(VarScope));
	vs->name     = name;
	vs->arg_size = size;
	vs->offset   = 0;

	list_insert(list_end(var), vs);

	return vs;
}