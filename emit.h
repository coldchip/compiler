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
	BC_MOD,
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
	BC_JNE,
	BC_JE,
	BC_JMP,
	BC_DEREF,
	BC_REF,
	BC_MOV,
	BC_MOVIND,
	BC_MOVIND2,
	BC_CMP,
	BC_SETEGT,
	BC_SETELT,
	BC_SETEEQ,
	BC_SETENEQ,
	BC_LEA,
	BC_SYSCALL,
	BC_HALT
} ByteCode;

typedef enum {
	BM_L = 1 << 0,
	BM_R = 1 << 1,
	BM_L_REG = 1 << 2,
	BM_R_REG = 1 << 3,
	BM_L_ADDR = 1 << 4,
	BM_R_ADDR = 1 << 5,
	BM_L_VAL = 1 << 6,
	BM_R_VAL = 1 << 7,
	BM_L_IND = 1 << 8,
	BM_R_IND = 1 << 9
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
	F_GT   = 19, // flag greater
	F_EQ   = 20, // flag equal
	F_LT   = 21, // flag less
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

typedef struct _Label {
	ListNode node;
	char *name;
	int addr;
} Label;

typedef struct _OP {
	ListNode node;
	ByteCode op;
	int left;
	int right;
	ByteMode mode;
	char *comments;
} OP;

typedef struct _Emit {
	unsigned constant_pool_index;
	List constant_pool;
	List code;
	List label;
} Emit;

Emit *new_emit();
int emit_add_to_constant_pool(Emit *emit, char *string, ConstantType type);
int emit_label(Emit *emit, char *format, ...);
unsigned emit_get_current_line(Emit *emit);
int emit_get_label_addr(Emit *emit, char *format, ...);
char *emit_get_label_name(Emit *emit, int addr);
OP *emit_opcode(Emit *emit, ByteMode mode, ByteCode op, int left, int right, char *comments);
void emit_asm(Emit *emit, char *file);
void emit_build2(Emit *emit, char *file);
void free_emit(Emit *emit);

#endif