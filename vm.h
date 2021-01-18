#ifndef VM_H
#define VM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef enum {
	NONE,
	R0,
	R1,
	R2,
	R3,
	R4,
	R5
} Reg;

typedef enum {
	PUSH,
	POP,
	MOV,
	ADD,
	JMP,
	
	INTERUPT,
	PUSHSTACK,
	POPSTACK,
} InsType;

typedef struct _OP {
	ListNode node;
	InsType ins;
	Reg left;
	Reg right;
	int rval;
	int lval;
} OP;

void exec();

void print_hex(const char *string);

#endif