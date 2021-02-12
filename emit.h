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
	BC_AND,
	BC_STRCONCAT,
	BC_STORE,
	BC_LOAD,
	BC_PUSH,
	BC_POP,
	BC_CALL,
	BC_RET,
	BC_CMPEQ,
	BC_CMPNEQ,
	BC_CMPGT,
	BC_CMPLT,
	BC_JMPIFEQ,
	BC_GOTO,
	BC_DEREF,
	BC_REF,
	BC_MOV
} ByteCode;

typedef enum {
	BM_L = 1 << 7,
	BM_R = 1 << 3,
	BM_L_REG = 1 << 6,
	BM_R_REG = 1 << 2,
	BM_L_ADDR = 1 << 5,
	BM_R_ADDR = 1 << 1,
	BM_L_VAL = 1 << 4,
	BM_R_VAL = 1 << 0,
} ByteMode;

typedef enum {
	REG_0  = 0,
	REG_1  = 1,
	REG_2  = 2,
	REG_3  = 3,
	REG_4  = 4,
	REG_5  = 5,
	REG_6  = 6,
	REG_7  = 7,
	REG_8  = 8,
	REG_9  = 9,
	REG_10 = 10,
	REG_11 = 11,
	REG_12 = 12,
	REG_13 = 13,
	REG_14 = 14,
	REG_15 = 15,
	SP     = 16, // stack pointer
	FP     = 17, // frame pointer
	IP     = 18, // instruction pointer
	REGSIZE
} Register;

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
	ByteMode mode;
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
OP *emit_opcode(Emit *emit, ByteMode mode, ByteCode op, int left, int right);
void emit_asm(Emit *emit, char *file);
void emit_build2(Emit *emit, char *file);
void free_emit(Emit *emit);

#endif