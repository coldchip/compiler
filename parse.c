#include <string.h>
#include "chipcode.h"

void parse_stmt(Parser *parser) {

}

void parse_param(Parser *parser) {
	parse_basetype(parser);
	parse_declarator(parser);
}

void parse_params(Parser *parser) {
	if(peek_string(parser, ")")) {
		return;
	}

	parse_param(parser);

	while(!peek_string(parser, ")")) {
		expect_string(parser, ","); consume(parser);
		parse_param(parser);
	}
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

void parse_declarator(Parser *parser) {
	expect_type(parser, TK_IDENT); consume(parser);
}

void parse_function(Parser *parser) {
	parse_basetype(parser);
	parse_declarator(parser);

	expect_string(parser, "("); consume(parser);

	parse_params(parser);

	expect_string(parser, ")"); consume(parser);

	expect_string(parser, "{"); consume(parser);

	expect_string(parser, "}"); consume(parser);
}

void parse_program(Parser *parser) {
	while(!peek_type(parser, TK_EOF)) {
		parse_function(parser);
	}
}

void parse(Token *token) {
	Parser parser;
	parser.token = token;
	parse_program(&parser);
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
	if(!peek_string(parser, str))
		c_error("Parse Error: Expecting %s at line %i", str, parser->token->line);
}

void expect_type(Parser *parser, TokenType type) {
	if(!peek_type(parser, type))
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