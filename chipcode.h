#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>

#define VERSION 1.0
#define API extern

// symtable.c

typedef struct _List {
	struct _ListEntry *entry;
	struct _ListEntry *start;
} List;

typedef struct _ListEntry {
	void *ptr;
	struct _ListEntry *next;
} ListEntry;

API List *list_init();
API List *list_clone(List *st);
API void list_add(List *st, void *ptr);
API void list_free(List *st);

// chipcode.c

API void c_error(char *format, ...);

// macro.c

API char *macro(char *input);

// lex.c
typedef enum {
	TK_IDENT,
	TK_NUMBER,
	TK_STRING,
	TK_SPECIAL,
	TK_EOF
} TokenType;

typedef struct _Token {
	char *data;
	int line;
	TokenType type;
	struct _Token *next;
} Token;

char *malloc_string_to_bit(char *data);
char *malloc_strcpy(char *data, int size);
void assert_not_eof(char *bit);
Token *new_token(Token *prev, TokenType type, char* data);
bool is_space(char bit);
bool startswith(char *p, char *q);
bool is_keyword(char bit);
bool is_number(char bit);
bool strmatch(char *a, char *b);
API Token *lex(char *file);

// parse.c

typedef enum {
	AST_PROGRAM,
	AST_FUNCTION
} NodeType;

typedef struct _Parser {
	Token *token;
} Parser;

typedef struct _Node {
	NodeType type;
	List *body;
} Node;

Node *new_node(NodeType type);

void parse_expr(Parser *parser);
void parse_assign(Parser *parser);
void parse_plus(Parser *parser);
void parse_minus(Parser *parser);
void parse_relational(Parser *parser);
void parse_primary(Parser *parser);

void parse_arg(Parser *parser);
void parse_args(Parser *parser);
void parse_param(Parser *parser);
void parse_params(Parser *parser);

void parse_call(Parser *parser);
void parse_stmt(Parser *parser);
void parse_declarator(Parser *parser);
void parse_basetype(Parser *parser);
Node *parse_function(Parser *parser);
Node *parse_program(Parser *parser);

bool is_function(Parser *parser);
bool is_call(Parser *parser);

bool consume_string(Parser *parser, const char *str);
bool consume_type(Parser *parser, TokenType type);
void expect_string(Parser *parser, const char *str);
void expect_type(Parser *parser, TokenType type);
bool peek_string(Parser *parser, const char *str);
bool peek_type(Parser *parser, TokenType type);

bool is_typename(Parser *parser);
API Node *parse(Token *token);

// codegen.c

void generate(Node *node);

// vm.c

typedef enum {
	OP_MOV,
	OP_JMP,
	OP_SUB,
	OP_DIV,
	OP_MUL,
	OP_ADD,
	OP_CALL,
	OP_CMP,
	OP_PUSH,
	OP_POP
} OpCode;

void vm_exec();

#endif