// ChipCode Intermediate Representation

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

IR *ir_init() {
	IR *ir = malloc(sizeof(IR));
	ir->entry = NULL;
	ir->start = NULL;
	return ir;
}

void ir_add_instruction(IR *ir, OPCode op, int left, int right) {
	InstructionEntry *next = malloc(sizeof(InstructionEntry));
	memset(next, 0, sizeof(InstructionEntry));

	next->op = op;
	next->left = left;
	next->right = right;

	if(ir->entry == NULL) {
		ir->start = next;
	} else {
		ir->entry->next = next;
	}
	ir->entry = next;
}