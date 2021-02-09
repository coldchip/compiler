#ifndef EMIT_H
#define EMIT_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <time.h>
#include "chipcode.h"
#include "list.h"

#define OBFUSCATION 1
#define OPTIMIZATION 1

typedef struct _Header {
	int magic;
	int version;
	uint64_t time;
} Header;

typedef enum {
	BC_NOP, // do nothing
	BC_NEWARRAY,
	BC_ARR_STORE,
	BC_ARR_LOAD,
	BC_ADD,
	BC_SUB,
	BC_MUL,
	BC_DIV,
	BC_SHL,
	BC_SHR,
	BC_STRCONCAT,
	BC_STORE,
	BC_LOAD,
	BC_PUSH_I, // for numbers and char
	BC_PUSH_S, // string
	BC_CALL,
	BC_RET,
	BC_CMPEQ,
	BC_CMPNEQ,
	BC_CMPGT,
	BC_CMPLT,
	BC_JMPIFEQ,
	BC_GOTO
} ByteCode; // max 64 bytecodes (00111111) 2 bits used for size

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

typedef struct _VarAddr {
	ListNode node;
	char *name;
	int addr;
} VarAddr;

typedef struct _Emit {
	unsigned constant_pool_index;
	List constant_pool;
	List functions;
	List vars;
	Function *current_function;
} Emit;

Emit *new_emit();
int emit_add_to_constant_pool(Emit *emit, char *string, ConstantType type);
void emit_select_function(Emit *emit, char *name);
unsigned emit_get_current_line(Emit *emit);
OP *emit_opcode_0(Emit *emit, ByteCode op);
OP *emit_opcode_1(Emit *emit, ByteCode op, int left);
OP *emit_opcode_2(Emit *emit, ByteCode op, int left, int right);
void emit_asm(Emit *emit, char *file);
void emit_build2(Emit *emit, char *file);
void free_emit(Emit *emit);

#endif