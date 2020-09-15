#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

OP *new_op(InsType ins, Reg left, Reg right, int lval, int rval) {
	OP *op = malloc(sizeof(OP));
	op->ins = ins;
	op->left = left;
	op->right = right;
	op->lval = lval;
	op->rval = rval;
	return op;
}

void exec() {
	List ins;
	list_clear(&ins);

	list_insert(list_end(&ins), new_op(PUSHSTACK, NONE, NONE, 0, 0));
	list_insert(list_end(&ins), new_op(PUSH, NONE, NONE, 256, 0));
	list_insert(list_end(&ins), new_op(PUSH, NONE, NONE, 128, 0));

	list_insert(list_end(&ins), new_op(MOV , R0, NONE, 0, 0));
	list_insert(list_end(&ins), new_op(PUSH , R0, NONE, 0, 0));

	list_insert(list_end(&ins), new_op(MOV , R0, NONE, 0, 4));
	list_insert(list_end(&ins), new_op(PUSH , R0, NONE, 0, 0));

	list_insert(list_end(&ins), new_op(POP , R0, NONE, 0, 0));
	list_insert(list_end(&ins), new_op(POP , R1, NONE, 0, 0));

	list_insert(list_end(&ins), new_op(ADD , R0, R1, 0, 0));

	list_insert(list_end(&ins), new_op(POPSTACK, NONE, NONE, 0, 0));

	printf("ColdChip VM 1.0\n");

	char *memory = malloc(1024 * 1024 * sizeof(char));
	int sp = 0x00;
	int fp = sp;

	int reg[10];

	for(ListNode *i = list_begin(&ins); i != list_end(&ins); i = list_next(i)) {
		OP *op = (OP*)i;

		switch(op->ins) {
			case PUSHSTACK: {
				fp = sp;
			}
			break;
			case POPSTACK: {
				sp = fp;
			}
			break;
			case PUSH: {
				if(op->left != NONE) {
					memcpy(memory + sp, (int*)&reg[op->left], sizeof(int));
				} else {
					memcpy(memory + sp, &op->lval, sizeof(int));
				}
				sp += sizeof(int);
			}
			break;
			case POP: {
				sp -= sizeof(int);
				if(op->left != NONE) {
					memcpy((int*)&reg[op->left], memory + sp, sizeof(int));
				}
			}
			break;
			case MOV: {
				if(op->left != NONE) {
					memcpy((int*)&reg[op->left], memory + fp + op->rval, sizeof(int));
				}
			}
			break;
			case ADD: {
				// Math should be only done in registers
				if(op->left != NONE && op->right != NONE) {
					reg[op->left] += reg[op->right];
				}
			}
			break;
		}
	}

	printf("REG: %i is %i\n", R0, reg[R0]);
	printf("REG: %i is %i\n", R1, reg[R1]);
	printf("REG: %i is %i\n", R2, reg[R2]);
	printf("REG: %i is %i\n", R3, reg[R3]);
	printf("REG: %i is %i\n", R4, reg[R4]);
	printf("REG: %i is %i\n", R5, reg[R5]);

	print_hex(memory);
}

void print_hex(const char *string) {
	unsigned char *p = (unsigned char *) string;

	for (int i=0; i < 200; ++i) {
	        if (! (i % 4) && i) {
	            printf(" RW\n");
	        }

	        printf("%02X", p[i] & 0xff);
	}
	printf("\n\n");
}