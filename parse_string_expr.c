#include "parse.h"

Node *parse_string_expr(Parser *parser) {
	Node *node = parse_string_assign(parser);
	return node;
}

/* Parses "aa" = "aa" ... */

Node *parse_string_assign(Parser *parser) {
	Node *left = parse_string_concat(parser);
	if(consume_string(parser, "=")) {
		Node *node = new_node(AST_ASSIGN);
		node->left = left;
		node->right = parse_string_assign(parser);
		return node;
	}
	return left;
}

/* Parses "aa" + "aa" */

Node *parse_string_concat(Parser *parser) {
	Node *left = parse_string_primary(parser);
	if(consume_string(parser, "+")) {
		Node *node = new_node(AST_STRING_CONCAT);
		node->left = left;
		node->right = parse_string_concat(parser);
		return node;
	}
	return left;
}

/* Parses "aa" */

Node *parse_string_primary(Parser *parser) {
	Token *token = parser->token;
	if(is_call(parser)) {
		return parse_call(parser);
	} else if(consume_type(parser, TK_STRING)) {
		Node *node = new_node(AST_STRING_LITERAL);
		node->token = token;
		return node;	
	} else if(consume_type(parser, TK_IDENT)) {
		if(consume_string(parser, "[")) {
			Node *node = new_node(AST_IDENT_MEMBER);
			node->index = parse_expr(parser);
			node->token = token;
			expect_string(parser, "]");
			return node;
		} else {
			Node *node = new_node(AST_IDENT);
			node->token = token;
			return node;
		}		
	} else {
		c_error("Expecting string at line %i, got %i", token->line, token->type);
	}
	return NULL;
}