#include "parse.h"

Node *parse_param(Parser *parser) {
	VarScope *vs = malloc(sizeof(VarScope));


	Token *token = parser->token;
	vs->name = token->data;

	parse_basetype(parser);
	expect_type(parser, TK_IDENT);
	
	Node *node = new_node(AST_IDENT);
	node->token = token;

	list_insert(list_end(&parser->varscope), vs);

	return node;
}

Node *parse_params(Parser *parser) {
	Node *node = new_node(AST_PARAM);
	if(peek_string(parser, ")")) {
		return node;
	}

	list_insert(list_end(&node->bodylist), parse_param(parser));

	while(!peek_string(parser, ")")) {
		expect_string(parser, ",");
		list_insert(list_end(&node->bodylist), parse_param(parser));
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