#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>

#define VERSION 1.0
#define API extern

// llist.c

typedef struct _List {
	struct _ListEntry *entry;
	struct _ListEntry *start;
} List;

typedef struct _ListEntry {
	void *ptr;
	struct _ListEntry *next;
	struct _ListEntry *prev;
} ListEntry;

API List *list_init();
API void list_push(List *st, void *ptr);
API void *list_pop(List *st);
API ListEntry *list_get_entry(List *st);
API void list_free_entry(ListEntry *le);
API void list_free(List *st);

// scope.c

typedef struct _Scope {
	List *list;
} Scope;

typedef struct _ScopeEntry {
	List *var;
} ScopeEntry;

API Scope *scope_init();
API void scope_push(Scope *scope);
API void scope_pop(Scope *scope);
void scope_entry_free(ScopeEntry *se);
void scope_add_var(Scope *scope, const char *var);
API void scope_free(Scope *st);

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
API void token_free(Token *token);

// parse.c

typedef enum {
	AST_PROGRAM,
	AST_FUNCTION,
	AST_IF,
	AST_BLOCK,
	AST_WHILE,
	AST_BINEXPR,
	AST_IDENT,
	AST_LITERAL,
	AST_DECL
} NodeType;

typedef struct _Parser {
	Token *token;
	Scope *scope;
} Parser;

typedef struct _Node {
	NodeType type;
	struct _Node *left;
	struct _Node *right;
	struct _Node *body;
	List *bodylist;
	Token *token;
} Node;

Node *new_node(NodeType type);
void node_free(Node *node);

Node *parse_expr(Parser *parser);
Node *parse_assign(Parser *parser);
Node *parse_plus(Parser *parser);
Node *parse_minus(Parser *parser);
Node *parse_relational(Parser *parser);
Node *parse_primary(Parser *parser);

void parse_arg(Parser *parser);
void parse_args(Parser *parser);
void parse_param(Parser *parser);
void parse_params(Parser *parser);

void parse_call(Parser *parser);
Node *parse_declaration(Parser *parser);
void parse_declarator(Parser *parser);
void parse_basetype(Parser *parser);
Node *parse_stmt(Parser *parser);
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

typedef enum {
	NO_REG,
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8
} Register;

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

void emit(OpCode op, Register a, Register b, int c, int d);

void enter_expr(Node *node);
void enter_function(Node *node);
void enter_program(Node *node);
void visitor(Node *node);
void generate(Node *node);

// vm.c

void vm_exec();

#endif