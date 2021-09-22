#ifndef VARLIST_H
#define VARLIST_H

#include <stdbool.h>
#include "parse.h"
#include "list.h"

typedef enum {
	VT_VARIABLE,
	VT_FUNCTION
} VarType;

typedef struct _VarScope {
	ListNode node;
	char *name;
	int offset;
	int size;
	DataType data_type;
	int arg_size;
} VarScope;

int var_get_total_allocated_size(List *var);
VarScope *var_get(List *var, char *name);
VarScope *var_insert(List *var, char *name, int size);
VarScope *var_insert_func(List *var, char *name, int size);

#endif