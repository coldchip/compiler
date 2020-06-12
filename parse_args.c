#include <string.h>
#include "chipcode.h"

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
		expect_string(parser, ",");
		parse_param(parser);
	}
}

void parse_arg(Parser *parser) {
	parse_declarator(parser);
}

void parse_args(Parser *parser) {
	if(peek_string(parser, ")")) {
		return;
	}

	parse_arg(parser);

	while(!peek_string(parser, ")")) {
		expect_string(parser, ",");
		parse_arg(parser);
	}
}