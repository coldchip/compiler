#include <string.h>
#include "chipcode.h"

Node *parse_param(Parser *parser) {
	parse_basetype(parser);
	Token *token = parser->token;
	expect_type(parser, TK_IDENT);
	Node *node = new_node(AST_IDENT);
	node->token = token;
	node->offset = scope_get_offset(parser->scope, token->data);

	if(scope_has_var(parser->scope, token->data)) {
		c_error("Conflicting variable \"%s\"", token->data);
	}

	scope_add_var(parser->scope, token->data, parser->scope->offset);	
	parser->scope->offset += 8;

	return node;
}

Node *parse_params(Parser *parser) {
	Node *node = new_node(AST_PARAM);
	if(peek_string(parser, ")")) {
		return node;
	}

	list_push(node->bodylist, parse_param(parser));

	while(!peek_string(parser, ")")) {
		expect_string(parser, ",");
		list_push(node->bodylist, parse_param(parser));
	}
	return node;
}

Node *parse_arg(Parser *parser) {
	Token *token = parser->token;
	expect_type(parser, TK_IDENT);
	if(!scope_has_var(parser->scope, token->data)) {
		c_error("Undefined variable \"%s\"", token->data);
	}
	Node *node = new_node(AST_IDENT);
	node->token = token;
	node->offset = scope_get_offset(parser->scope, token->data);
	return node;
}

Node *parse_args(Parser *parser) {
	Node *node = new_node(AST_ARG);
	if(peek_string(parser, ")")) {
		return node;
	}

	list_push(node->bodylist, parse_arg(parser));

	while(!peek_string(parser, ")")) {
		expect_string(parser, ",");
		list_push(node->bodylist, parse_arg(parser));
	}
	return node;
}