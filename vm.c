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

void put_reg(Process *process, char *reg_or_value, uint64_t data) {
	if(strcmp(reg_or_value, "fp") == 0) {
		process->fp = data;
	} else if(strcmp(reg_or_value, "sp") == 0) {
		process->sp = data;
	} else if(strcmp(reg_or_value, "r0") == 0) {
		process->r0 = data;
	} else if(strcmp(reg_or_value, "r1") == 0) {
		process->r1 = data;
	} else if(strcmp(reg_or_value, "r2") == 0) {
		process->r2 = data;
	} else if(strcmp(reg_or_value, "r3") == 0) {
		process->r3 = data;
	} else {
		c_error("Put error");
	}
}

uint64_t get_reg(Process *process, char *a) {
	char reg[64];
	strcpy(reg, a);
	char *reg_or_value = strtok(reg, "+");
	char *offset_char = strtok(NULL, "+");

	int offset = 0;
	if(offset_char) {
		offset = atoi(offset_char);
	}
	if(strcmp(reg_or_value, "fp") == 0) {
		return process->fp + offset;
	} else if(strcmp(reg_or_value, "sp") == 0) {
		return process->sp + offset;
	} else if(strcmp(reg_or_value, "r0") == 0) {
		return process->r0 + offset;
	} else if(strcmp(reg_or_value, "r1") == 0) {
		return process->r1 + offset;
	} else if(strcmp(reg_or_value, "r2") == 0) {
		return process->r2 + offset;
	} else if(strcmp(reg_or_value, "r3") == 0) {
		return process->r3 + offset;
	} else {
		process->val = atoi(reg_or_value);
		return process->val;
	}

}

void op_exec(Process *process, char *op, char *a, char *b) {
	if(a && b) {
		printf("%s %s, %s\n", op, a, b);
	} else {
		printf("%s %s\n", op, a);
	}
	if(strcmp(op, "push") == 0) {
		*((uint64_t*)process->sp) = get_reg(process, a);
		process->sp += 8;
	}
	if(strcmp(op, "pop") == 0) {
		process->sp -= 8;
		uint64_t num;
		memcpy(&num, (void*)process->sp, 8);
		put_reg(process, a, num);
	}
	if(strcmp(op, "mov") == 0) {
		put_reg(process, a, get_reg(process, b));
	}
	if(strcmp(op, "lea") == 0) {
		uint64_t *ptr = (void*)get_reg(process, b);
		put_reg(process, a, *ptr);
	}
	if(strcmp(op, "add") == 0) {
		uint64_t left  = get_reg(process, a);
		uint64_t right = get_reg(process, b);
		uint64_t total = left + right;
		put_reg(process, a, total);
	}
	if(strcmp(op, "sub") == 0) {
		uint64_t left  = get_reg(process, a);
		uint64_t right = get_reg(process, b);
		uint64_t total = left - right;
		put_reg(process, a, total);
	}
	if(strcmp(op, "mul") == 0) {
		uint64_t left  = get_reg(process, a);
		uint64_t right = get_reg(process, b);
		uint64_t total = left * right;
		put_reg(process, a, total);
	}
	if(strcmp(op, "div") == 0) {
		uint64_t left  = get_reg(process, a);
		uint64_t right = get_reg(process, b);
		uint64_t total = left / right;
		put_reg(process, a, total);
	}
}

void free_process(Process *process) {
	free(process->stack);
	free(process);
}

void print_hex(const char *string) {
	unsigned char *p = (unsigned char *) string;

	for (int i=0; i < 200; ++i) {
	        if (! (i % 8) && i) {
	            printf(" RW\n");
	        }

	        printf("%02x", p[i]);
	}
	printf("\n\n");
}