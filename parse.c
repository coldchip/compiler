#include <string.h>
#include <stdlib.h>
#include "chipcode.h"

Node *new_node(NodeType type) {
	Node *node = malloc(sizeof(Node));
	node->bodylist = list_init();
	node->left = NULL;
	node->right = NULL;
	node->type = type;
	return node;
}

void node_free(Node *node) {
	if(node->bodylist) {
		list_free(node->bodylist);
	}
	free(node);
}

void parse_call(Parser *parser) {
	expect_type(parser, TK_IDENT);
	expect_string(parser, "(");
	parse_args(parser);
	expect_string(parser, ")");
	expect_string(parser, ";");
}

void parse_basetype(Parser *parser) {
	if(peek_string(parser, "int") || peek_string(parser, "void")) {
		if(consume_string(parser, "int")) {

		} else if(consume_string(parser, "void")) {

		}
	} else {
		c_error("Invalid base type");
	}
}

Node *parse_declaration(Parser *parser) {
	Node *node = new_node(AST_DECL);
	parse_basetype(parser);
	node->body = parse_expr(parser);
	return node;
}

void parse_declarator(Parser *parser) {
	expect_type(parser, TK_IDENT);
}

Node *parse_stmt(Parser *parser) {
	if(consume_string(parser, "if")) {
		expect_string(parser, "(");
		parse_expr(parser);
		expect_string(parser, ")");
		parse_stmt(parser);
		if(consume_string(parser, "else")) {
			parse_stmt(parser);
		}

		Node *node = new_node(AST_IF);
		return node;
	} else if(consume_string(parser, "{")) {

		Node *node = new_node(AST_BLOCK);

		while(!peek_string(parser, "}")) {
			list_push(node->bodylist, parse_stmt(parser));
		}
		expect_string(parser, "}");

		return node;
	} else if(consume_string(parser, "while")) {
		expect_string(parser, "(");
		parse_expr(parser);
		expect_string(parser, ")");
		parse_stmt(parser);

		Node *node = new_node(AST_WHILE);
		return node;
	} else if(is_typename(parser)) {
		Node *node = parse_declaration(parser);
		expect_string(parser, ";");
		return node;
	} else {
		Node *node = parse_expr(parser);
		expect_string(parser, ";");
		return node;
	}
}

Node *parse_function(Parser *parser) {

	scope_push(parser->scope);

	Node *node = new_node(AST_FUNCTION);

	parse_basetype(parser);
	parse_declarator(parser);

	expect_string(parser, "(");

	parse_params(parser);

	expect_string(parser, ")");

	expect_string(parser, "{");

	while(!peek_string(parser, "}")) {
		list_push(node->bodylist, parse_stmt(parser));
	}

	expect_string(parser, "}");

	scope_pop(parser->scope);

	return node;
}

Node *parse_program(Parser *parser) {

	scope_push(parser->scope);

	Node *node = new_node(AST_PROGRAM);

	while(!peek_type(parser, TK_EOF)) {
		if(is_function(parser)) {
			list_push(node->bodylist, parse_function(parser));
		} else {

		}
	}

	scope_pop(parser->scope);

	return node;
}

Node *parse(Token *token) {
	Parser parser;
	parser.token = token;
	parser.scope = scope_init();

	Node *node = parse_program(&parser);
	
	scope_free(parser.scope);
	return node;
}

bool is_function(Parser *parser) {
	Parser rewind = *parser;
	parse_basetype(&rewind);
	parse_declarator(&rewind);
	if(consume_string(&rewind, "(")) {
		return true;
	}
	return false;
}

bool is_call(Parser *parser) {
	Parser rewind = *parser;
	if(consume_type(&rewind, TK_IDENT) && consume_string(&rewind, "(")) {
		return true;
	}
	return false;
}

void consume(Parser *parser) {
	parser->token = parser->token->next;
}

bool consume_string(Parser *parser, const char *str) {
	if(peek_string(parser, str)) {
		parser->token = parser->token->next;
		return true;
	} else {
		return false;
	}
}

bool consume_type(Parser *parser, TokenType type) {
	if(peek_type(parser, type)) {
		parser->token = parser->token->next;
		return true;
	} else {
		return false;
	}
}

void expect_string(Parser *parser, const char *str) {
	if(!consume_string(parser, str))
		c_error("Parse Error: Expecting %s at line %i, got \"%s\"", str, parser->token->line, parser->token->data);
}

void expect_type(Parser *parser, TokenType type) {
	if(!consume_type(parser, type))
		c_error("Parse Error: Expecting type %i at line %i", type, parser->token->line);
}

bool peek_type(Parser *parser, TokenType type) {
	return parser->token->type == type;
}

bool peek_string(Parser *parser, const char *str) {
	return (strcmp(parser->token->data, str) == 0);
}

bool is_typename(Parser *parser) {
	return 
	peek_string(parser, "void") || 
	peek_string(parser, "int") || 
	peek_string(parser, "char");
}