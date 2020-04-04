// ChipCode Intermediate Representation

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

IR *ir_init() {
	IR *ir = malloc(sizeof(IR));
	ir->i_entry = NULL;
	ir->i_start = NULL;
	ir->c_entry = NULL;
	ir->c_start = NULL;
	return ir;
}

void ir_add_constant(IR *ir, char *data) {
	ConstantEntry *next = malloc(sizeof(ConstantEntry));
	memset(next, 0, sizeof(ConstantEntry));

	char *data_malloc = malloc((sizeof(char) * strlen(data)) + 1);
	strcpy(data_malloc, data);

	next->data = data_malloc;

	if(ir->c_entry == NULL) {
		ir->c_start = next;
	} else {
		ir->c_entry->next = next;
	}
	ir->c_entry = next;
}