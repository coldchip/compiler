#include <string.h>
#include "chipcode.h"

void parse_expr(Parser *parser) {
	parse_assign(parser);
}

void parse_assign(Parser *parser) {
	parse_plus(parser);
	if(consume_string(parser, "=")) {
		parse_assign(parser);
	}
}

void parse_plus(Parser *parser) {
	parse_minus(parser);
	if(consume_string(parser, "+")) {
		parse_plus(parser);
	}
}

void parse_minus(Parser *parser) {
	parse_relational(parser);
	if(consume_string(parser, "-")) {
		parse_minus(parser);
	}
}

void parse_relational(Parser *parser) {
	parse_primary(parser);
	if(consume_string(parser, "<")) {
		parse_relational(parser);
	} else if(consume_string(parser, "<=")) {
		parse_relational(parser);
	} else if(consume_string(parser, ">")) {
		parse_relational(parser);
	} else if(consume_string(parser, ">=")) {
		parse_relational(parser);
	}
}

void parse_primary(Parser *parser) {
	if(!consume_type(parser, TK_IDENT) && !consume_type(parser, TK_NUMBER)) {
		c_error("Expecting Identifier or Literal at Line %i", parser->token->line);
	}
}