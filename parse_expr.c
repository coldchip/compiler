#include <string.h>
#include "chipcode.h"

Node *parse_expr(Parser *parser) {
	Node *node = parse_assign(parser);
	return node;
}

Node *parse_assign(Parser *parser) {
	Node *left = parse_plus(parser);
	if(consume_string(parser, "=")) {
		Node *node = new_node(AST_BINEXPR);
		node->left = left;
		node->right = parse_assign(parser);
		return node;
	}
	return left;
}

Node *parse_plus(Parser *parser) {
	Node *left = parse_minus(parser);
	if(consume_string(parser, "+")) {
		Node *node = new_node(AST_BINEXPR);
		node->left = left;
		node->right = parse_plus(parser);
		return node;
	}
	return left;
}

Node *parse_minus(Parser *parser) {
	Node *left = parse_relational(parser);
	if(consume_string(parser, "-")) {
		Node *node = new_node(AST_BINEXPR);
		node->left = left;
		node->right = parse_minus(parser);
		return node;
	}
	return left;
}

Node *parse_relational(Parser *parser) {
	Node *left = parse_primary(parser);
	if(consume_string(parser, "<")) {
		Node *node = new_node(AST_BINEXPR);
		node->left = left;
		node->right = parse_relational(parser);
		return node;
	}
	return left;
}

Node *parse_primary(Parser *parser) {
	Token *token = parser->token;
	if(consume_type(parser, TK_IDENT)) {
		if(consume_string(parser, "(")) {
			return parse_call(parser);
		} else {
			if(!scope_has_var(parser->scope, token->data)) {
				c_error("Undefined variable \"%s\"", token->data);
			}
			Node *node = new_node(AST_IDENT);
			node->token = token;
			return node;
		}
	} else if(consume_type(parser, TK_NUMBER)) {
		Node *node = new_node(AST_LITERAL);
		node->token = token;
		return node;
	} else {
		c_error("Expecting Identifier or Literal at Line %i", parser->token->line);
	}
	return NULL;
}