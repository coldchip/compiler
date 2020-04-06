/*
	ColdChip inc. 
	@Author Ryan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include "chipcode.h"

static void c_error(char *format, ...) {
	va_list args;
    va_start(args, format);

	char fmt[1000];
	snprintf(fmt, sizeof(fmt), "[PARSER] %s\n", format);
	vprintf(fmt, args);
    
    va_end(args);
	exit(1);
}

TokenType get_token_type(ParseState *ps) {
	return ps->token->type;
}

int get_line(ParseState *ps) {
	return ps->token->line;
}

char* get_string_from_token(ParseState *ps) {
	return ps->token->string;
}

char* get_string_from_node(ASTNode *node) {
	return node->token->string;
}

bool is(ParseState *ps, TokenType type) {
	if(get_token_type(ps) == type) {
		return true;
	}
	return false;
}

void next(ParseState *ps) {
	ps->token = ps->token->next;
}

void expect(ParseState *ps, TokenType type) {
	if(get_token_type(ps) != type) {
		c_error("Unexpected %s at line %i", get_string_from_token(ps), get_line(ps));
	}
}

static ASTNode *new_node(NodeType type) {
	// Updates previous entry->next pointer with the malloc'd entry pointer
	ASTNode *node = malloc(sizeof(ASTNode));
	memset(node, 0, sizeof(Token));
	node->type = type;
	node->next = NULL;
	node->body = NULL;
	return node;
}

void add_body(ASTNode *node, ASTNode *body) {
	if(node->body == NULL) {
		node->body = body;
	} else {
		ASTNode *current = node->body;
		while(1) {
			if(current->next == NULL) {
				current->next = body;
				break;
			} else {
				current = current->next;
			}
		}
	}
}

void parse_type(ParseState *ps) {
	if(is(ps, KW_VOID)) {
		next(ps);
	} else {
		c_error("%s is not a type at line %i", get_string_from_token(ps), get_line(ps));
	}
}

ASTNode *parse_identifier(ParseState *ps) {
	ASTNode *node = new_node(AST_IDENTIFIER);
	if(is(ps, IDENTIFIER)) {
		node->token = ps->token;
		next(ps);
		return node;
	} else {
		c_error("%s is not an IDENTIFIER at line %i", get_string_from_token(ps), get_line(ps));
	}
	return NULL;
}

ASTNode *parse_literal(ParseState *ps) {
	ASTNode *node = new_node(AST_LITERAL);
	if(is(ps, NUMBER) || is(ps, STRING)) {
		node->token = ps->token;
		next(ps);
		return node;
	} else {
		c_error("%s is not an LITERAL at line %i", get_string_from_token(ps), get_line(ps));
	}
	return NULL;
}

ASTNode *parse_expr(ParseState *ps) {
	ASTNode *left;
	switch(get_token_type(ps)) {
		case IDENTIFIER:
		{
			left = parse_identifier(ps);
		}
		break;
		case NUMBER:
		{
			left = parse_literal(ps);
		}
		break;
		default:
		{
			c_error("Unknown type %s at line %i", get_string_from_token(ps), get_line(ps));
		}
		break;
	}
	if(is(ps, PLUS)) {
		next(ps);
		ASTNode *right = parse_expr(ps);

		ASTNode *expr = new_node(AST_BIN_EXPR);
		expr->left = left;
		expr->right = right;
		return expr;
	} else {
		return left;
	}
}

ASTNode *parse_decl(ParseState *ps) {
	if(is(ps, KW_VAR)) {
		ASTNode *node = new_node(AST_DECLARATOR);
		next(ps);
		node->left = parse_identifier(ps);
		if(symtable_has(ps->st, get_string_from_node(node->left)) == true) {
			c_error("Error, Variable %s already existed. ", get_string_from_node(node->left));
		}
		symtable_add(ps->st, get_string_from_node(node->left), 0);
		if(!is(ps, SEMICOLON)) {
			expect(ps, ASSIGN);
			next(ps);
			switch(get_token_type(ps)) {
				case STRING:
				{
					node->right = parse_literal(ps);
				}
				break;
				case IDENTIFIER:
				{
					node->right = parse_identifier(ps);
					if(symtable_has(ps->st, get_string_from_node(node->right)) != true) {
						c_error("Variable %s not defined", get_string_from_node(node->right));
					}
				}
				break;
				case NUMBER:
				{
					node->right = parse_expr(ps);
				}
				break;
				default:
				{
					c_error("Right hand side of declarator is not a valid type at line %i", get_line(ps));
				}
				break;
			}
			expect(ps, SEMICOLON);
			next(ps);
			return node;
		} else {
			next(ps);
			return node;
		}
	} else {
		c_error("Unknown type %s at line %i", get_string_from_token(ps), get_line(ps));
	}
	return NULL;
}

ASTNode *parse_block(ParseState *ps) {
	SymbolTable *st_orig  = ps->st;
	ps->st = symtable_clone(ps->st);

	ASTNode *node = new_node(AST_BLOCK);
	expect(ps, LBRACE);
	next(ps);
	while(!is(ps, RBRACE)) {
		add_body(node, parse_stmt(ps));
	}
	expect(ps, RBRACE);
	next(ps);

	symtable_free(ps->st);
	ps->st = st_orig;

	return node;
}

ASTNode *parse_argument(ParseState *ps) {
	ASTNode *node = new_node(AST_ARGUMENT);
	while(is(ps, IDENTIFIER)) {
		add_body(node, parse_identifier(ps));
		symtable_add(ps->st, get_string_from_node(node->body), 0);
		if(is(ps, COMMA)) {
			expect(ps, COMMA);
			next(ps);
			expect(ps, IDENTIFIER);
		}
	}
	return node;
}

ASTNode *parse_parameter(ParseState *ps) {
	ASTNode *node = new_node(AST_PARAMETER);
	while(is(ps, IDENTIFIER) || is(ps, NUMBER) || is(ps, STRING)) {
		if(is(ps, IDENTIFIER)) {
			ASTNode *ident = parse_identifier(ps);
			add_body(node, ident);
			if(symtable_has(ps->st, get_string_from_node(ident)) != true) {
				c_error("Undefined parameter variable \"%s\"", get_string_from_node(ident));
			}
		} else if(is(ps, NUMBER) || is(ps, STRING)) {
			add_body(node, parse_literal(ps));
		} else {
			c_error("Param only accepts identifier and literal");
		}
		if(is(ps, COMMA)) {
			expect(ps, COMMA);
			next(ps);
		}
	}
	return node;
}

ASTNode *parse_call(ParseState *ps) {
	ASTNode *node = new_node(AST_CALL);
	node->identifier = parse_identifier(ps);
	if(strcmp(get_string_from_node(node->identifier), "__call__") != 0) {
		if(symtable_has(ps->st, get_string_from_node(node->identifier)) != true) {
			c_error("Call to undefined function \"%s\"", get_string_from_node(node->identifier));
		}
	}
	expect(ps, LPAREN);
	next(ps);
	node->args = parse_parameter(ps);
	expect(ps, RPAREN);
	next(ps);
	expect(ps, SEMICOLON);
	next(ps);
	return node;
}

ASTNode *parse_stmt(ParseState *ps) {
	switch(get_token_type(ps)) {
		case LBRACE:
		{
			return parse_block(ps);
		}
		break;
		case KW_VAR:
		{
			return parse_decl(ps);
		}
		break;
		case IDENTIFIER:
		{
			return parse_call(ps);
		}
		break;
		default:
		{
			c_error("%s is not a valid statement at line %i", get_string_from_token(ps), get_line(ps));
		}
		break;
	}
	return NULL;
}

ASTNode *parse_function(ParseState *ps) {
	ASTNode *node = new_node(AST_FUNCTION);

	parse_type(ps);
	node->identifier = parse_identifier(ps);
	if(symtable_has(ps->st, get_string_from_node(node->identifier)) == true) {
		c_error("Duplicate of function \"%s\"", get_string_from_node(node->identifier));
	}
	symtable_add(ps->st, get_string_from_node(node->identifier), 0);
	
	SymbolTable *st_orig  = ps->st;
	ps->st = symtable_clone(ps->st);

	expect(ps, LPAREN);
	next(ps);
	node->args = parse_argument(ps);
	expect(ps, RPAREN);
	next(ps);

	expect(ps, LBRACE);
	next(ps);
	while(!is(ps, RBRACE)) {
		add_body(node, parse_stmt(ps));
	}
	expect(ps, RBRACE);
	next(ps);

	symtable_free(ps->st);
	ps->st = st_orig;

	return node;
}

ASTNode *parse(Token *token) {
	/*
	Token *token_copy = token;
	while(token_copy != NULL) {
		if(token_copy->type != END_OF_TOKEN) {
			printf("Line: %i Type: %i Token: %s\n", token_copy->line, (int)token_copy->type, token_copy->string);
		}
		token_copy = token_copy->next;
	}
	*/
	ParseState ps = {};
	ps.token = token;
	ps.st = symtable_init();

	ASTNode *node = new_node(AST_PROGRAM);

	while(!is(&ps, END_OF_TOKEN)) {
		add_body(node, parse_function(&ps));
	}

	symtable_free(ps.st);

	return node;
}