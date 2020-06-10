#include <string.h>
#include "chipcode.h"

void parse_function(Parser *parser) {
	if(is_typename(parser)) {
		c_error("Passed");
	} else {
		c_error("Expecting type");
	}
}

void parse_program(Parser *parser) {
	while(!is_type(parser, TK_EOF)) {
		parse_function(parser);
	}
}

void parse(Token *token) {
	Parser parser;
	parser.token = token;
	parse_program(&parser);
}

void expect(Parser *parser, const char *str) {
	if(!is_data(parser, str))
		c_error("Parse Error: Expecting %s at line %i", str, parser->token->line);
}

void next(Parser *parser) {
	parser->token = parser->token->next;
}

bool is_type(Parser *parser, TokenType type) {
	return parser->token->type == type;
}

bool is_data(Parser *parser, const char *str) {
	return (strcmp(parser->token->data, str) == 0);
}

bool is_typename(Parser *parser) {
	return 
	is_data(parser, "void") || 
	is_data(parser, "int") || 
	is_data(parser, "char");
}