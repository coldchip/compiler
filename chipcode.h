#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>
#include <stdint.h>

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
API List *list_clone(List *list);
API void list_push(List *st, void *ptr);
API void *list_pop(List *st);
API void list_free_entry(ListEntry *le);
API void list_free(List *st);

// scope.c

typedef struct _Scope {
	List *var;
} Scope;

API Scope *scope_init();
API Scope *scope_clone(Scope *scope);
API void scope_add_var(Scope *scope, const char *var);
API bool scope_has_var(Scope *scope, const char *var);
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
	AST_ASSIGN,
	AST_ADD,
	AST_SUB,
	AST_MUL,
	AST_DIV,
	AST_LT,
	AST_EQUAL,
	AST_IDENT,
	AST_LITERAL,
	AST_DECL,
	AST_CALL
} NodeType;

typedef struct _Parser {
	Token *token;
	Scope *scope;
} Parser;

typedef struct _Node {
	NodeType type;
	struct _Node *condition;

	struct _Node *left;
	struct _Node *right;

	struct _Node *body;
	struct _Node *alternate;

	List *bodylist;
	Token *token;
} Node;

Node *new_node(NodeType type);
void node_free(Node *node);

Node *parse_expr(Parser *parser);
Node *parse_assign(Parser *parser);
Node *parse_plus(Parser *parser);
Node *parse_minus(Parser *parser);
Node *parse_muliply(Parser *parser);
Node *parse_divide(Parser *parser);
Node *parse_relational(Parser *parser);
Node *parse_equality(Parser *parser);
Node *parse_primary(Parser *parser);

void parse_arg(Parser *parser);
void parse_args(Parser *parser);
void parse_param(Parser *parser);
void parse_params(Parser *parser);

Node *parse_call(Parser *parser);
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

// vm.c

typedef struct _Process {
	uint64_t sp;
	uint64_t fp;
	uint64_t r0;
	uint64_t r1;
	uint64_t r2;
	uint64_t r3;
	char *stack;

	uint64_t val;
} Process;

API Process *new_process();

void *get_reg(Process *process, char *a);

API void op_exec(Process *process, char *op, char *a, char *b);

API void print_hex(const char *string);

API void free_process(Process *process);

// codegen.c

typedef struct _Generator {
	Process *process;
} Generator;

void enter_program(Generator *generator, Node *node);
void enter_function(Generator *generator, Node *node);
void enter_block(Generator *generator, Node *node);
void enter_decl(Generator *generator, Node *node);
void enter_binexpr(Generator *generator, Node *node);
void enter_literal(Generator *generator, Node *node);
void enter_ident(Generator *generator, Node *node);
void enter_call(Generator *generator, Node *node);
void enter_if(Generator *generator, Node *node);
void enter_while(Generator *generator, Node *node);
void visitor(Generator *generator, Node *node);
void generate(Node *node);

#endif