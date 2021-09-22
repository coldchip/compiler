#include "varlist.h"
#include "type.h"
#include "parse.h"

Node *parse_expr(Parser *parser) {
	Node *node = parse_assign(parser);
	return node;
}

Node *parse_assign(Parser *parser) {
	Node *node = parse_log(parser);
	if(consume_string(parser, "=")) {
		node = new_binary(AST_ASSIGN, node, parse_assign(parser));
		return node;
	}
	return node;
}

Node *parse_log(Parser *parser) {
	Node *node = parse_equality(parser);
	for(;;) {
		if(consume_string(parser, "&&")) {
			return new_binary(AST_LOGAND, node, parse_equality(parser));
		}
		return node;
	}
}

Node *parse_equality(Parser *parser) {
	Node *node = parse_relational(parser);
	for(;;) {
		if(consume_string(parser, "==")) {
			node = new_binary(AST_EQUAL, node, parse_relational(parser));
			continue;
		}
		if(consume_string(parser, "!=")) {
			node = new_binary(AST_NOTEQUAL, node, parse_relational(parser));
			continue;
		}
		return node;
	}
}

Node *parse_relational(Parser *parser) {
	Node *node = parse_bitwise(parser);
	for(;;) {
		if(consume_string(parser, "<")) {
			node = new_binary(AST_LT, node, parse_bitwise(parser));
			continue;
		}
		if(consume_string(parser, ">")) {
			node = new_binary(AST_LT, node, parse_bitwise(parser));
			continue;
		}
		return node;
	}
}

Node *parse_bitwise(Parser *parser) {
	Node *node = parse_plus_minus(parser);
	for(;;) {
		if(consume_string(parser, "<<")) {
			node = new_binary(AST_SHL, node, parse_plus_minus(parser));
			continue;
		}
		if(consume_string(parser, ">>")) {
			node = new_binary(AST_SHR, node, parse_plus_minus(parser));
			continue;
		}
		if(consume_string(parser, "&")) {
			node = new_binary(AST_AND, node, parse_plus_minus(parser));
			continue;
		}
		return node;
	}
}

Node *parse_plus_minus(Parser *parser) {
	Node *node = parse_muliply_divide(parser);
	for(;;) {
		if(consume_string(parser, "+")) {
			node = new_binary_normalize_type(AST_ADD, node, parse_muliply_divide(parser));
			continue;
		}
		if(consume_string(parser, "-")) {
			node = new_binary_normalize_type(AST_SUB, node, parse_muliply_divide(parser));
			continue;
		}
		return node;
	}
}

Node *parse_muliply_divide(Parser *parser) {
	Node *node = parse_cast(parser);
	for(;;) {
		if(consume_string(parser, "*")) {
			node = new_binary(AST_MUL, node, parse_cast(parser));
			continue;
		}
		if(consume_string(parser, "/")) {
			node = new_binary(AST_DIV, node, parse_cast(parser));
			continue;
		}
		if(consume_string(parser, "%")) {
			node = new_binary(AST_MOD, node, parse_cast(parser));
			continue;
		}
		return node;
	}
}

Node *parse_cast(Parser *parser) {
	if(consume_string(parser, "(")) {
		if(is_typename(parser)) {
			DataType type = parse_basetype(parser);
			expect_string(parser, ")");

			return new_cast(parse_cast(parser), type);
		}
		unconsume(parser);
	}
	return parse_unary(parser);
}

Node *parse_unary(Parser *parser) {
	if(consume_string(parser, "&")) {
		Node *node = new_node(AST_REF);
		node->body = parse_unary(parser);
		return node;
	}
	if(consume_string(parser, "*")) {
		Node *node = new_node(AST_DEREF);
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
			VarScope *vs = var_get(&parser->varlist, token->data);
			if(!vs) {
				c_error("unable to find var '%s'", token->data);
			}
			node->token = token;
			node->offset = vs->offset;
			node->data_type = vs->data_type;

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
		if(strstr(token->data, ".") != NULL) {
			// typeof float
			node->data_type = DATA_FLOAT;
		} else {
			node->data_type = DATA_INT;
		}
		printf("======%i %s\n", node->data_type, token->data);
		return node;
	} else if(consume_type(parser, TK_STRING)) {
		Node *node = new_node(AST_STRING_LITERAL);
		node->token = token;
		return node;
	} else {
		c_error("Expecting Identifier or Literal at Line %i, got %i", token->line, token->type);
	}
	return NULL;
}