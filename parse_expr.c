#include "parse.h"

Node *parse_expr(Parser *parser) {
	Node *node = parse_assign(parser);
	return node;
}

Node *parse_assign(Parser *parser) {
	Node *left = parse_plus(parser);
	if(consume_string(parser, "=")) {
		Node *node = new_node(AST_ASSIGN);
		node->left = left;
		node->right = parse_assign(parser);
		return node;
	}
	return left;
}

Node *parse_plus(Parser *parser) {
	Node *left = parse_minus(parser);
	if(consume_string(parser, "+")) {
		Node *node = new_node(AST_ADD);
		node->left = left;
		node->right = parse_plus(parser);
		return node;
	}
	return left;
}

Node *parse_minus(Parser *parser) {
	Node *left = parse_muliply(parser);
	if(consume_string(parser, "-")) {
		Node *node = new_node(AST_SUB);
		node->left = left;
		node->right = parse_minus(parser);
		return node;
	}
	return left;
}

Node *parse_muliply(Parser *parser) {
	Node *left = parse_divide(parser);
	if(consume_string(parser, "*")) {
		Node *node = new_node(AST_MUL);
		node->left = left;
		node->right = parse_muliply(parser);
		return node;
	}
	return left;
}

Node *parse_divide(Parser *parser) {
	Node *left = parse_relational(parser);
	if(consume_string(parser, "/")) {
		Node *node = new_node(AST_DIV);
		node->left = left;
		node->right = parse_divide(parser);
		return node;
	}
	return left;
}

Node *parse_relational(Parser *parser) {
	Node *left = parse_equality(parser);
	if(consume_string(parser, "<")) {
		Node *node = new_node(AST_LT);
		node->left = left;
		node->right = parse_relational(parser);
		return node;
	}
	return left;
}

Node *parse_equality(Parser *parser) {
	Node *left = parse_primary(parser);
	if(consume_string(parser, "==")) {
		Node *node = new_node(AST_EQUAL);
		node->left = left;
		node->right = parse_equality(parser);
		return node;
	}
	return left;
}


Node *parse_primary(Parser *parser) {
	Token *token = parser->token;
	if(is_call(parser)) {
		return parse_call(parser);
	} else if(consume_type(parser, TK_IDENT)) {
		Node *node = new_node(AST_IDENT);
		node->token = token;
		return node;	
	} else if(consume_type(parser, TK_NUMBER)) {
		Node *node = new_node(AST_LITERAL);
		node->token = token;
		return node;
	} else {
		c_error("Expecting Identifier or Literal at Line %i, got %i", token->line, token->type);
	}
	return NULL;
}