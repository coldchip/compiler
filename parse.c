#include "parse.h"

Node *new_node(NodeType type) {
	Node *node = malloc(sizeof(Node));
	memset(node, 0, sizeof(Node));
	list_clear(&node->bodylist);
	node->type = type;
	return node;
}

void node_free(Node *node) {
	free(node);
}

int parse_get_offset(Parser *parser) {
	if(list_size(&parser->varscope) > 0) {
		VarScope *vs = (VarScope*)list_back(&parser->varscope);
		return vs->offset + vs->size;
	}
	return 0;
}

int parse_get_var_offset(Parser *parser, char *var) {
	for(ListNode *i = list_begin(&parser->varscope); i != list_end(&parser->varscope); i = list_next(i)) {
		VarScope *vs = (VarScope*)i;
		if(strcmp(var, vs->name) == 0) {
			return vs->offset;
		}
	}
	return 0;
}

bool parse_has_var(Parser *parser, char *var) {
	for(ListNode *i = list_begin(&parser->varscope); i != list_end(&parser->varscope); i = list_next(i)) {
		VarScope *vs = (VarScope*)i;
		if(strcmp(var, vs->name) == 0) {
			return true;
		}
	}
	return false;
}

Node *parse_call(Parser *parser) {
	Node *node = new_node(AST_CALL);
	Token *ident = parser->token;
	node->token = ident;
	expect_type(parser, TK_IDENT);
	expect_string(parser, "(");
	node->args = parse_args(parser);
	expect_string(parser, ")");
	return node;
}

DataType parse_basetype(Parser *parser) {
	DataType type = DATA_VOID;
	if(consume_string(parser, "int")) {
		type = DATA_INT;
	} else if(consume_string(parser, "long")) {
		type = DATA_LONG;
	} else if(consume_string(parser, "void")) {
		type = DATA_VOID;
	} else if(consume_string(parser, "char")) {
		type = DATA_CHAR;
	} else {
		c_error("Invalid base type");
	}

	if(consume_string(parser, "[")) {
		// type |= DATA_ARRAY_MASK;
		expect_string(parser, "]");
	}
	return type;
}

/* Parses any string/number declaration */

Node *parse_declaration(Parser *parser) {

	VarScope *vs = malloc(sizeof(VarScope));

	Node *node = new_node(AST_DECL);

	vs->size   = parse_basetype(parser);
	vs->offset = parse_get_offset(parser);


	Token *token = parser->token;

	if(parse_has_var(parser, token->data)) {
		c_error("redefinition of '%s'", token->data);
	}

	vs->name = token->data;
	expect_type(parser, TK_IDENT);
	node->token = token;

	/*
	if(node->data_type & DATA_ARRAY_MASK) {
		// = [10]
		expect_string(parser, "=");
		if(consume_string(parser, "[")) {
			node->offset = parser->total_local_size; // Set decl offset
			parser->total_local_size += atoi(parser->token->data); // increase offset
			expect_type(parser, DATA_NUMBER);
			expect_string(parser, "]");
		} else {
			node->body = parse_string_expr(parser);
		}
	} else {
		*/
		node->offset = vs->offset; // Set decl offset
		node->size = vs->size; // Set decl size
		if(consume_string(parser, "=")) {
			node->body = parse_expr(parser);	
		}
		/*
	}
	*/

	list_insert(list_end(&parser->varscope), vs);
	
	return node;
}

void parse_declarator(Parser *parser) {
	expect_type(parser, TK_IDENT);
}

Node *parse_stmt(Parser *parser) {
	if(consume_string(parser, "if")) {
		Node *node = new_node(AST_IF);

		expect_string(parser, "(");
		node->condition = parse_expr(parser);
		expect_string(parser, ")");
		node->body = parse_stmt(parser);
		if(consume_string(parser, "else")) {
			node->alternate = parse_stmt(parser);
		}
		return node;
	} else if(consume_string(parser, "{")) {

		Node *node = new_node(AST_BLOCK);

		while(!peek_string(parser, "}")) {
			list_insert(list_end(&node->bodylist), parse_stmt(parser));
		}
		expect_string(parser, "}");


		return node;
	} else if(consume_string(parser, "while")) {
		Node *node = new_node(AST_WHILE);
		expect_string(parser, "(");
		node->condition = parse_expr(parser);
		expect_string(parser, ")");
		node->body = parse_stmt(parser);
		return node;
	} else if(consume_string(parser, "return")) {
		Node *node = new_node(AST_RETURN);
		node->body = parse_expr(parser);
		expect_string(parser, ";");
		return node;
	} else if(is_typename(parser)) {
		Node *node = parse_declaration(parser);
		expect_string(parser, ";");
		return node;
	} else {
		Node *node = parse_expr(parser);
		expect_string(parser, ";");
		return node;
	}
}

Node *parse_function(Parser *parser) {
	Node *node = new_node(AST_FUNCTION);

	parse_basetype(parser);
	
	Token *ident = parser->token;
	node->token = ident;
	parse_declarator(parser);

	expect_string(parser, "(");

	node->args = parse_params(parser);

	expect_string(parser, ")");

	expect_string(parser, "{");

	list_clear(&parser->varscope);

	while(!peek_string(parser, "}")) {
		list_insert(list_end(&node->bodylist), parse_stmt(parser));
	}

	node->total_local_size = 0;

	while(!list_empty(&parser->varscope)) {
		VarScope *entry = (VarScope*)list_remove(list_back(&parser->varscope)); // begin with end
		node->total_local_size += entry->size;
	}

	expect_string(parser, "}");

	return node;
}

Node *parse_program(Parser *parser) {

	Node *node = new_node(AST_PROGRAM);

	while(!peek_type(parser, TK_EOF)) {
		if(is_function(parser)) {
			list_insert(list_end(&node->bodylist), parse_function(parser));
		} else {
			c_error("Not a function!");
		}
	}

	return node;
}

Node *parse(List *token) {
	Parser parser;
	parser.token = (Token*)list_begin(token);

	Node *node = parse_program(&parser);
	
	return node;
}

bool is_call(Parser *parser) {
	if(consume_type(parser, TK_IDENT)) {
		if(consume_string(parser, "(")) {
			unconsume(parser); unconsume(parser);
			return true;
		}
		unconsume(parser);
	}
	return false;
}

bool is_function(Parser *parser) {
	if(is_typename(parser)) {
		consume(parser);
		if(consume_type(parser, TK_IDENT)) {
			if(consume_string(parser, "(")) {
				unconsume(parser); unconsume(parser); unconsume(parser);
				return true;
			}
			unconsume(parser);
		}
		unconsume(parser);
	}
	return false;
}

void unconsume(Parser *parser) {
	parser->token = (Token*)list_previous((ListNode*)parser->token);
}

void consume(Parser *parser) {
	parser->token = (Token*)list_next((ListNode*)parser->token);
}

bool consume_string(Parser *parser, const char *str) {
	if(peek_string(parser, str)) {
		consume(parser);
		return true;
	} else {
		return false;
	}
}

bool consume_type(Parser *parser, TokenType type) {
	if(peek_type(parser, type)) {
		consume(parser);
		return true;
	} else {
		return false;
	}
}

void expect_string(Parser *parser, const char *str) {
	if(!consume_string(parser, str)) {
		Token *token = parser->token;
		if(token->type == TK_EOF) {
			c_error("Parse Error: Expecting %s at line %i, but end of file is reached", str, token->line, token->data);
		} else {
			c_error("Parse Error: Expecting %s at line %i, got \"%s\"", str, token->line, token->data);
		}
	}
}

void expect_type(Parser *parser, TokenType type) {
	if(!consume_type(parser, type)) {
		Token *token = parser->token;
		if(token->type == TK_EOF) {
			c_error("Parse Error: Expecting type %i at line %i, but end of file is reached", type, token->line);
		} else {
			c_error("Parse Error: Expecting type %i at line %i, got %i", type, token->line, token->type);
		}
	}
}

bool peek_type(Parser *parser, TokenType type) {
	Token *token = parser->token;
	if(token && token->type == type) {
		return true;
	}
	return false;
}

bool peek_string(Parser *parser, const char *str) {
	Token *token = parser->token;
	if(token && token->data && strcmp(token->data, str) == 0) {
		return true;
	} 
	return false;
}

bool is_typename(Parser *parser) {
	return 
	peek_string(parser, "void") || 
	peek_string(parser, "int") || 
	peek_string(parser, "char");
}