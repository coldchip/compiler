#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>

#define API extern

typedef struct _ChipBinary {
	char magic[8];
	int version;
	unsigned int timestamp;
} ChipBinary;

// lex.c
typedef enum {
	END_OF_TOKEN,
	IDENTIFIER,
	STRING,
	NUMBER,
	KW_VAR,
	KW_VOID,
	LPAREN,
	RPAREN,
	LBRACKET,
	RBRACKET,
	LBRACE,
	RBRACE,
	SEMICOLON,
	COMMA,
	ASSIGN,
	PLUS,
	MINUS,
	STAR,
	DIVIDE,
	LESST,
	MORET
} TokenType;

typedef struct _Token {
	char *string;
	int line;
	TokenType type;
	struct _Token *next;
} Token;

char *malloc_string_to_bit(char *data);
char *malloc_strcpy(char *data, int size);
extern Token *lex(char *file);

// symtable.c

typedef struct _SymbolTable {
	struct _TableEntry *entry;
	struct _TableEntry *start;
} SymbolTable;

typedef struct _TableEntry {
	char * name;
	int    pointer;
	struct _TableEntry *next;
} TableEntry;

API SymbolTable *symtable_init();
API SymbolTable *symtable_clone(SymbolTable *st);
API void symtable_add(SymbolTable *st, char *name, int pointer);
API bool symtable_has(SymbolTable *st, char *name);
API int  symtable_ptr(SymbolTable *st, char *name);
API void symtable_free(SymbolTable *st);

// macro.c

char *macro(char *input);

// parse.c
typedef enum {
	AST_PROGRAM,
	AST_FUNCTION,
	AST_PARAMETER,
	AST_ARGUMENT,
	AST_BIN_EXPR,
	AST_DECLARATOR,
	AST_IDENTIFIER,
	AST_LITERAL,
	AST_BLOCK,
	AST_IF,
	KW_ELSE,
	AST_WHILE,
	AST_CALL
} NodeType;

typedef struct _ASTNode {
	NodeType type;
	Token *token;
	struct _ASTNode *identifier;
	struct _ASTNode *condition;
	struct _ASTNode *body;
	struct _ASTNode *alternate;
	struct _ASTNode *left;
	TokenType op;
	struct _ASTNode *right;
	struct _ASTNode *args;
	struct _ASTNode *next;
} ASTNode;

typedef struct _ParseState {
	Token *token;
	SymbolTable *st;
} ParseState;

ASTNode *parse(Token *token);
ASTNode *parse_stmt(ParseState *ps);

// ir.c

typedef enum {
	NOP,
	ACOP,
	IMOV,
	CALL
} OPCode;

typedef struct _IR {
	
} IR;

API IR *ir_init();
API void ir_add_instruction(IR *ir, char* instruction, ...);

// generate.c

typedef struct _GenState {
	SymbolTable *st;
	IR *ir;
	int sp;
} GenState;

void *visitor(GenState *gs, ASTNode *node);

GenState *generate(ASTNode *node);


#endif