#include "varlist.h"
#include "parse.h"

Node *parse_param(Parser *parser) {
	DataType type = parse_basetype(parser);

	Token *token = parser->token;
	VarScope *vs = var_insert(&parser->varlist, token->data, (type & 0xFF));

	expect_type(parser, TK_IDENT);
	
	Node *node  = new_node(AST_IDENT);
	node->token = token;
	node->size  = vs->size; 

	return node;
}

Node *parse_params(Parser *parser) {
	Node *node = new_node(AST_PARAM);
	if(peek_string(parser, ")")) {
		return node;
	}

	Node *param = parse_param(parser);
	node->size += param->size;
	list_insert(list_end(&node->bodylist), param);

	while(!peek_string(parser, ")")) {
		expect_string(parser, ",");
		Node *param = parse_param(parser);
		node->size += param->size;
		list_insert(list_end(&node->bodylist), param);
	}
	return node;
}

Node *parse_arg(Parser *parser) {
	Node *node = parse_expr(parser);
	return node;
}

Node *parse_args(Parser *parser) {
	Node *node = new_node(AST_ARG);
	if(peek_string(parser, ")")) {
		return node;
	}

	list_insert(list_end(&node->bodylist), parse_arg(parser));

	while(!peek_string(parser, ")")) {
		expect_string(parser, ",");
		list_insert(list_end(&node->bodylist), parse_arg(parser));
	}
	return node;
}