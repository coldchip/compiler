#include "varlist.h"
#include "type.h"
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

Node *new_binary(NodeType type, Node *left, Node *right) {
	Node *node = new_node(type);
	node->left  = left;
	node->right = right;

	return node;
}

Node *new_binary_promote(NodeType type, Node *left, Node *right) {
	promote_type(left);
	promote_type(right);
	Node *node = new_binary(type, left, right);
	return node;
}

Node *new_cast(Node *from, DataType type) {
	promote_type(from);

	Node *node = new_node(AST_CAST);
	node->data_type = type;
	node->left = from;
	return node;
}

Node *parse_call(Parser *parser) {
	Node *node = new_node(AST_CALL);
	Token *token = parser->token;
	node->token = token;

	VarScope *vs = var_get(&parser->varlist, token->data);
	if(!vs && !strcmp(token->data, "__asm__") == 0) {
		c_error("unable to find function '%s'", token->data);
	}
	if(vs) {
		node->data_type = vs->data_type;
	}

	expect_type(parser, TK_IDENT);
	expect_string(parser, "(");
	node->args = parse_args(parser);
	if(!strcmp(token->data, "__asm__") == 0) {
		node->size = vs->arg_size;
	}
	expect_string(parser, ")");
	return node;
}

DataType parse_basetype(Parser *parser) {
	DataType type = DATA_VOID;
	if(consume_string(parser, "int")) {
		type = DATA_INT;
	} else if(consume_string(parser, "float")) {
		type = DATA_FLOAT;
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
	} else if(consume_string(parser, "*")) {
		type = 8;
	}
	return type;
}

/* Parses any string/number declaration */

Node *parse_declaration(Parser *parser) {
	Node *node = new_node(AST_DECL);

	DataType type = parse_basetype(parser);

	Token *token = parser->token;

	if(var_get(&parser->varlist, token->data)) {
		c_error("redefinition of '%s'", token->data);
	}

	VarScope *vs = var_insert(&parser->varlist, token->data, (type & 0xFF));
	vs->data_type = type;

	expect_type(parser, TK_IDENT);
	node->token = token;
	node->offset = vs->offset;
	node->data_type = type;
	node->size = vs->size; // Set decl size

	if(consume_string(parser, "=")) {
		node->body = parse_expr(parser);
		promote_type(node->body);
	}
	return node;
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
		if(!peek_string(parser, ";")) {
			node->body = parse_expr(parser);
		}
		expect_string(parser, ";");
		return node;
	} else if(is_typename(parser)) {
		Node *node = parse_declaration(parser);
		expect_string(parser, ";");
		return node;
	} else {
		Node *node = parse_expr(parser);
		promote_type(node);
		expect_string(parser, ";");
		return node;
	}
}

Node *parse_function(Parser *parser) {
	Node *node = new_node(AST_FUNCTION);

	DataType type = parse_basetype(parser);

	node->size = type & 0xFF;

	Token *token = parser->token;
	node->token = token;

	VarScope *vs = var_insert_func(&parser->varlist, token->data, 0);
	vs->data_type = type;

	expect_type(parser, TK_IDENT);

	expect_string(parser, "(");

	int varlist_size = list_size(&parser->varlist); // remember size before function

	node->args = parse_params(parser);

	vs->arg_size = node->args->size; // param size

	expect_string(parser, ")");

	expect_string(parser, "{");

	while(!peek_string(parser, "}")) {
		list_insert(list_end(&node->bodylist), parse_stmt(parser));
	}

	while(list_size(&parser->varlist) > varlist_size) {
		list_remove(list_back(&parser->varlist)); // restore size after function
	}

	expect_string(parser, "}");

	return node;
}

Node *parse_program(Parser *parser) {
	list_clear(&parser->varlist);

	Node *node = new_node(AST_PROGRAM);

	while(!peek_type(parser, TK_EOF)) {
		if(is_function(parser)) {
			list_insert(list_end(&node->bodylist), parse_function(parser));
		} else if(consume_string(parser, "struct")) {
			expect_type(parser, TK_IDENT);
			expect_string(parser, "{");
			
			expect_string(parser, "}");
			expect_string(parser, ";");
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
	peek_string(parser, "float") || 
	peek_string(parser, "char") || 
	peek_string(parser, "long");
}