#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>

#define API extern

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
} ParseState;

ASTNode *parse(Token *token);
ASTNode *parse_stmt(ParseState *ps);

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

// generate.c

typedef struct _GenState {
	SymbolTable *st;
	int sp;
} GenState;

void *visitor(GenState *gs, ASTNode *node);

void generate(GenState *gs, ASTNode *node);



#endif