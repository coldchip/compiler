#ifndef EMIT_H
#define EMIT_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "chipcode.h"
#include "list.h"

typedef enum {
	BC_PUSHSTR,
	BC_STRCONCAT,
	BC_STORE,
	BC_LOAD,
	BC_PUSH,
	BC_ADD,
	BC_SUB,
	BC_MUL,
	BC_DIV,
	BC_CALL
} ByteCode;

typedef struct _ConstantPoolRow {
	ListNode node;
	unsigned index;
	char *data;
} ConstantPoolRow;

typedef struct _OP {
	ListNode node;
	ByteCode op;
	int left;
	int right;
} OP;

typedef struct _Emit {
	unsigned constant_pool_index;
	List constant_pool;
	List code;
} Emit;

Emit *new_emit();
int emit_add_to_constant_pool(Emit *emit, char *string);
void emit_opcode(Emit *emit, ByteCode op, int left, int right);
void emit_build(Emit *emit, char *file);
void free_emit(Emit *emit);

#endif