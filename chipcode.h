#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define VERSION 1.0
#define API extern

// llist.c

typedef struct _ListNode
{
   struct _ListNode * next;
   struct _ListNode * previous;
} ListNode;

typedef struct _List
{
   ListNode sentinel;
} List;

extern void list_clear (List *);

extern ListNode * list_insert (ListNode *, void *);
extern void * list_remove (ListNode *);
extern ListNode * list_move (ListNode *, void *, void *);

extern size_t list_size (List *);

#define list_begin(list) ((list) -> sentinel.next)
#define list_end(list) (& (list) -> sentinel)

#define list_empty(list) (list_begin (list) == list_end (list))

#define list_next(iterator) ((iterator) -> next)
#define list_previous(iterator) ((iterator) -> previous)

#define list_front(list) ((void *) (list) -> sentinel.next)
#define list_back(list) ((void *) (list) -> sentinel.previous)

// scope.c


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
	ListNode node;
	char *data;
	int line;
	TokenType type;
} Token;

char *malloc_string_to_bit(char *data);
char *malloc_strcpy(char *data, int size);
void assert_not_eof(char *bit);
Token *new_token(TokenType type, char* data, int line);
bool is_space(char bit);
bool startswith(char *p, char *q);
bool is_keyword(char bit);
bool is_number(char bit);
bool strmatch(char *a, char *b);
API void lex(List *list, char *file);
API void token_free(List *list);

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
	AST_CALL,
	AST_RETURN,
	AST_ARG,
	AST_PARAM
} NodeType;

typedef struct _Parser {
	Token *token;
} Parser;

typedef struct _Node {
	ListNode node;
	NodeType type;
	struct _Node *condition;

	struct _Node *left;
	struct _Node *right;

	struct _Node *body;
	struct _Node *alternate;

	struct _Node *args;

	uint64_t offset;

	List bodylist;
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

Node *parse_arg(Parser *parser);
Node *parse_args(Parser *parser);
Node *parse_param(Parser *parser);
Node *parse_params(Parser *parser);

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
API Node *parse(List *token);

// vm.c

API void print_hex(const char *string);


// codegen.c

typedef struct _Generator {
	FILE *file;
} Generator;

void emit(Generator *generator, const char *op, const char *a, const char *b);
void emit_procedure(Generator *generator, const char *label);
void emit_param(Generator *generator, const char *label);

void gen_store(Generator *generator);
void gen_addr(Generator *generator, Node *node);

void enter_assign(Generator *generator, Node *node);
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
void enter_param(Generator *generator, Node *node);
void enter_arg(Generator *generator, Node *node);
void enter_return(Generator *generator, Node *node);
void visitor(Generator *generator, Node *node);
void generate(Node *node);

#endif