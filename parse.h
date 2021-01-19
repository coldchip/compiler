#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include "lex.h"

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
	AST_PARAM,
	AST_STRING_CONCAT,
	AST_STRING
} NodeType;

typedef enum {
	DATA_STRING,
	DATA_NUMBER,
	DATA_VOID
} DataType;

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

// parse_string_expr.c

Node *parse_string_expr(Parser *parser);
Node *parse_string_assign(Parser *parser);
Node *parse_string_concat(Parser *parser);
Node *parse_string_primary(Parser *parser);

// parse_expr.c

Node *parse_expr(Parser *parser);
Node *parse_assign(Parser *parser);
Node *parse_plus(Parser *parser);
Node *parse_minus(Parser *parser);
Node *parse_muliply(Parser *parser);
Node *parse_divide(Parser *parser);
Node *parse_relational(Parser *parser);
Node *parse_equality(Parser *parser);
Node *parse_primary(Parser *parser);

// parse_args.c

Node *parse_arg(Parser *parser);
Node *parse_args(Parser *parser);
Node *parse_param(Parser *parser);
Node *parse_params(Parser *parser);

// parse.c

Node *parse_call(Parser *parser);
Node *parse_declaration(Parser *parser);
void parse_declarator(Parser *parser);
DataType parse_basetype(Parser *parser);
Node *parse_stmt(Parser *parser);
Node *parse_function(Parser *parser);
Node *parse_program(Parser *parser);

bool is_function(Parser *parser);
bool is_call(Parser *parser);

void consume(Parser *parser);
bool consume_string(Parser *parser, const char *str);
bool consume_type(Parser *parser, TokenType type);
void expect_string(Parser *parser, const char *str);
void expect_type(Parser *parser, TokenType type);
bool peek_string(Parser *parser, const char *str);
bool peek_type(Parser *parser, TokenType type);

bool is_typename(Parser *parser);
Node *parse(List *token);

#endif