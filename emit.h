#ifndef EMIT_H
#define EMIT_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <time.h>
#include "chipcode.h"
#include "list.h"

typedef struct _Header {
	int magic;
	int version;
	uint64_t time;
} Header;

typedef enum {
	BC_STRCONCAT,
	BC_STORE,
	BC_LOAD,
	BC_PUSH_I, // for numbers and char
	BC_PUSH_S,
	BC_ADD,
	BC_SUB,
	BC_MUL,
	BC_DIV,
	BC_CALL,
	BC_RET,
	BC_CMPEQ,
	BC_CMPNEQ,
	BC_CMPGT,
	BC_CMPLT,
	BC_JMPIFEQ,
	BC_GOTO,
	BC_NEWARRAY,
	BC_ARR_STORE,
	BC_ARR_LOAD
} ByteCode;

typedef enum {
	CT_VARIABLE,
	CT_STRING,
	CT_FNAME
} ConstantType;

typedef struct _ConstantPoolRow {
	ListNode node;
	unsigned index;
	char *data;
	ConstantType type;
} ConstantPoolRow;

typedef struct _OP {
	ListNode node;
	ByteCode op;
	int left;
	int right;
	int size;
} OP;

typedef struct _Function {
	ListNode node;
	char *name;
	List code;
} Function;

typedef struct _Emit {
	unsigned constant_pool_index;
	List constant_pool;
	List functions;
	Function *current_function;
} Emit;

Emit *new_emit();
int emit_add_to_constant_pool(Emit *emit, char *string, ConstantType type);
void emit_select_function(Emit *emit, char *name);
unsigned emit_get_current_line(Emit *emit);
OP *emit_opcode(Emit *emit, ByteCode op, int left, int right);
void emit_build(Emit *emit, char *file);
void free_emit(Emit *emit);

#endif