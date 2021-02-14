#include "parse.h"

Node *parse_expr(Parser *parser) {
	Node *node = parse_assign(parser);
	return node;
}

Node *parse_assign(Parser *parser) {
	Node *left = parse_plus_minus(parser);
	if(consume_string(parser, "=")) {
		Node *node = new_node(AST_ASSIGN);
		node->left = left;
		node->right = parse_assign(parser);
		return node;
	}
	return left;
}

Node *parse_plus_minus(Parser *parser) {
	Node *left = parse_muliply_divide(parser);
	for(;;) {
		if(consume_string(parser, "+")) {
			Node *node = new_node(AST_ADD);
			node->left = left;
			node->right = parse_muliply_divide(parser);
			left = node;
			continue;
		}
		if(consume_string(parser, "-")) {
			Node *node = new_node(AST_SUB);
			node->left = left;
			node->right = parse_muliply_divide(parser);
			left = node;
			continue;
		}
		return left;
	}
}

Node *parse_muliply_divide(Parser *parser) {
	Node *left = parse_relational(parser);
	for(;;) {
		if(consume_string(parser, "*")) {
			Node *node = new_node(AST_MUL);
			node->left = left;
			node->right = parse_relational(parser);
			left = node;
			continue;
		}
		if(consume_string(parser, "/")) {
			Node *node = new_node(AST_DIV);
			node->left = left;
			node->right = parse_relational(parser);
			left = node;
			continue;
		}
		return left;
	}
}

Node *parse_relational(Parser *parser) {
	Node *left = parse_bitwise(parser);
	for(;;) {
		if(consume_string(parser, "<")) {
			Node *node = new_node(AST_LT);
			node->left = left;
			node->right = parse_bitwise(parser);
			left = node;
			continue;
		}
		if(consume_string(parser, ">")) {
			Node *node = new_node(AST_GT);
			node->left = left;
			node->right = parse_bitwise(parser);
			left = node;
			continue;
		}
		return left;
	}
}

Node *parse_bitwise(Parser *parser) {
	Node *left = parse_equality(parser);
	for(;;) {
		if(consume_string(parser, "<<")) {
			Node *node = new_node(AST_SHL);
			node->left = left;
			node->right = parse_equality(parser);
			left = node;
			continue;
		}
		if(consume_string(parser, ">>")) {
			Node *node = new_node(AST_SHR);
			node->left = left;
			node->right = parse_equality(parser);
			left = node;
			continue;
		}
		if(consume_string(parser, "&")) {
			Node *node = new_node(AST_AND);
			node->left = left;
			node->right = parse_equality(parser);
			left = node;
			continue;
		}
		return left;
	}
}

Node *parse_equality(Parser *parser) {
	Node *left = parse_unary(parser);
	for(;;) {
		if(consume_string(parser, "==")) {
			Node *node = new_node(AST_EQUAL);
			node->left = left;
			node->right = parse_unary(parser);
			left = node;
			continue;
		}
		if(consume_string(parser, "!=")) {
			Node *node = new_node(AST_NOTEQUAL);
			node->left = left;
			node->right = parse_unary(parser);
			left = node;
			continue;
		}
		return left;
	}
}

Node *parse_unary(Parser *parser) {
	if(consume_string(parser, "&")) {
		Node *node = new_node(AST_DEREF);
		node->body = parse_unary(parser);
		return node;
	}
	if(consume_string(parser, "*")) {
		Node *node = new_node(AST_REF);
		node->body = parse_unary(parser);
		return node;
	}
	return parse_primary(parser);
}

Node *parse_primary(Parser *parser) {
	Token *token = parser->token;
	if(is_call(parser)) {
		return parse_call(parser);
	} else if(consume_type(parser, TK_IDENT)) {
		if(consume_string(parser, "[")) {
			Node *node = new_node(AST_IDENT_MEMBER);
			node->index = parse_expr(parser);
			node->token = token;
			expect_string(parser, "]");
			return node;
		} else {
			Node *node = new_node(AST_IDENT);
			if(!parse_has_var(parser, token->data)) {
				c_error("unable to find var '%s'", token->data);
			}
			int offset = parse_get_var_offset(parser, token->data);
			node->token = token;
			node->offset = offset;
			return node;
		}	
	} else if(consume_string(parser, "(")) {
		Node *node = parse_expr(parser);
		expect_string(parser, ")");
		return node;
	} else if(consume_type(parser, TK_CHAR)) {
		Node *node = new_node(AST_CHAR_LITERAL);
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