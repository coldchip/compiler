#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

Process *new_process() {
	Process *process = malloc(sizeof(Process));
	process->fp = 0;
	process->stack = malloc(1024 * 1024 * sizeof(char));
	process->sp = (uint64_t)process->stack;
	printf("%p\n", process->stack);
	return process;
}

void *get_reg(Process *process, char *a) {
	if(strcmp(a, "fp") == 0) {
		return &process->fp;
	} else if(strcmp(a, "sp") == 0) {
		return &process->sp;
	} else if(strcmp(a, "r0") == 0) {
		return &process->r0;
	} else if(strcmp(a, "r1") == 0) {
		return &process->r1;
	} else if(strcmp(a, "r2") == 0) {
		return &process->r2;
	} else if(strcmp(a, "r3") == 0) {
		return &process->r3;
	} else {
		process->val = atoi(a);
		return &process->val;
	}
	return NULL;

}

void op_exec(Process *process, char *op, char *a, char *b) {
	if(strcmp(op, "push") == 0) {
		memcpy((void*)process->sp, get_reg(process, a), 8);
		process->sp += 8;
	}
	if(strcmp(op, "pop") == 0) {
		process->sp -= 8;
		memcpy(get_reg(process, a), (void*)process->sp, 8);
	}
	if(strcmp(op, "mov") == 0) {
		memcpy(get_reg(process, a), get_reg(process, b), 8);
	}
	if(strcmp(op, "add") == 0) {
		uint64_t *left  = get_reg(process, a);
		uint64_t *right = get_reg(process, b);
		uint64_t total = *left + *right;
		memcpy(get_reg(process, a), &total, 8);
	}
	printf("Stack Data: \n");
	print_hex(process->stack);
}

void print_hex(const char *string) {
	unsigned char *p = (unsigned char *) string;

	for (int i=0; i < 200; ++i) {
	        if (! (i % 8) && i)
	                printf("\n");

	        printf("%02x", p[i]);
	}
	printf("\n\n");
}