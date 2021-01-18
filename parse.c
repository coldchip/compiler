#include <string.h>
#include <stdlib.h>
#include "parse.h"

Node *new_node(NodeType type) {
	Node *node = malloc(sizeof(Node));
	list_clear(&node->bodylist);
	node->left = NULL;
	node->right = NULL;
	node->type = type;
	return node;
}

void node_free(Node *node) {
	List *body = &node->bodylist;
	while(!list_empty(body)) {
		Node *current = (Node*)list_remove(list_begin(body));
		
	}
	free(node);
}

Node *parse_call(Parser *parser) {
	Node *node = new_node(AST_CALL);
	Token *ident = parser->token;
	node->token = ident;
	expect_type(parser, TK_IDENT);
	expect_string(parser, "(");
	node->args = parse_args(parser);
	expect_string(parser, ")");
	return node;
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

	Token *token = parser->token;
	
	expect_type(parser, TK_IDENT);

	if(peek_string(parser, "[")) {
		consume_string(parser, "[");
		expect_string(parser, "]");
		consume_string(parser, "]");
	}
	
	if(consume_string(parser, "=")) {
		node->body = parse_expr(parser);
	}
	return node;
}

void parse_declarator(Parser *parser) {
	expect_type(parser, TK_IDENT);
}

Node *parse_stmt(Parser *parser) {
	if(consume_string(parser, "if")) {
		Node *node = new_node(AST_IF);

		expect_string(parser, "(");
		node->condition = parse_expr(parser);
		expect_string(parser, ")");
		node->body = parse_stmt(parser);
		if(consume_string(parser, "else")) {
			node->alternate = parse_stmt(parser);
		}

		return node;
	} else if(consume_string(parser, "{")) {

		Node *node = new_node(AST_BLOCK);

		while(!peek_string(parser, "}")) {
			list_insert(list_end(&node->bodylist), parse_stmt(parser));
		}
		expect_string(parser, "}");


		return node;
	} else if(consume_string(parser, "while")) {
		Node *node = new_node(AST_WHILE);
		expect_string(parser, "(");
		node->condition = parse_expr(parser);
		expect_string(parser, ")");
		node->body = parse_stmt(parser);
		return node;
	} else if(consume_string(parser, "return")) {
		Node *node = new_node(AST_RETURN);
		node->body = parse_expr(parser);
		expect_string(parser, ";");
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
	Node *node = new_node(AST_FUNCTION);

	parse_basetype(parser);
	
	Token *ident = parser->token;
	node->token = ident;
	parse_declarator(parser);

	expect_string(parser, "(");

	node->args = parse_params(parser);

	expect_string(parser, ")");

	expect_string(parser, "{");

	while(!peek_string(parser, "}")) {
		list_insert(list_end(&node->bodylist), parse_stmt(parser));
	}

	expect_string(parser, "}");

	return node;
}

Node *parse_program(Parser *parser) {

	Node *node = new_node(AST_PROGRAM);

	while(!peek_type(parser, TK_EOF)) {
		if(is_function(parser)) {
			list_insert(list_end(&node->bodylist), parse_function(parser));
		} else {
			c_error("Not a function!");
		}
	}

	return node;
}

Node *parse(List *token) {
	Parser parser;
	parser.token = (Token*)list_begin(token);

	Node *node = parse_program(&parser);
	
	return node;
}

bool is_call(Parser *parser) {
	Parser rewind = *parser;
	if(consume_type(&rewind, TK_IDENT) && consume_string(&rewind, "(")) {
		return true;
	}
	return false;
}

bool is_function(Parser *parser) {
	Parser rewind = *parser;
	if(is_typename(&rewind)) {
		consume(&rewind);
		if(consume_type(&rewind, TK_IDENT) && consume_string(&rewind, "(")) {
			return true;
		}
	}
	return false;
}

void consume(Parser *parser) {
	parser->token = (Token*)list_next((ListNode*)parser->token);
}

bool consume_string(Parser *parser, const char *str) {
	if(peek_string(parser, str)) {
		consume(parser);
		return true;
	} else {
		return false;
	}
}

bool consume_type(Parser *parser, TokenType type) {
	if(peek_type(parser, type)) {
		consume(parser);
		return true;
	} else {
		return false;
	}
}

void expect_string(Parser *parser, const char *str) {
	if(!consume_string(parser, str)) {
		Token *token = parser->token;
		if(token->type == TK_EOF) {
			c_error("Parse Error: Expecting %s at line %i, but end of file is reached", str, token->line, token->data);
		} else {
			c_error("Parse Error: Expecting %s at line %i, got \"%s\"", str, token->line, token->data);
		}
	}
}

void expect_type(Parser *parser, TokenType type) {
	if(!consume_type(parser, type)) {
		Token *token = parser->token;
		if(token->type == TK_EOF) {
			c_error("Parse Error: Expecting type %i at line %i, but end of file is reached", type, token->line);
		} else {
			c_error("Parse Error: Expecting type %i at line %i, got %i", type, token->line, token->type);
		}
	}
}

bool peek_type(Parser *parser, TokenType type) {
	Token *token = parser->token;
	if(token && token->type == type) {
		return true;
	}
	return false;
}

bool peek_string(Parser *parser, const char *str) {
	Token *token = parser->token;
	if(token && token->data && strcmp(token->data, str) == 0) {
		return true;
	} 
	return false;
}

bool is_typename(Parser *parser) {
	return 
	peek_string(parser, "void") || 
	peek_string(parser, "int") || 
	peek_string(parser, "char");
}