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

TokenType get_type(ParseState *ps) {
	return ps->token->type;
}

bool is(ParseState *ps, TokenType type) {
	if(get_type(ps) == type) {
		return true;
	}
	return false;
}

void next(ParseState *ps) {
	ps->token = ps->token->next;
	if(is(ps, END_OF_TOKEN)) {
		c_error("Unexpected End of File reached at line %i", ps->token->line);
	}
}

void expect(ParseState *ps, TokenType type) {
	if(get_type(ps) != type) {
		c_error("Unexpected %s at line %i", ps->token->string, ps->token->line);
	}
}

static ASTNode *new_node(NodeType type) {
	// Updates previous entry->next pointer with the malloc'd entry pointer
	ASTNode *node = malloc(sizeof(ASTNode));
	memset(node, 0, sizeof(Token));
	node->type = type;
	node->next = NULL;
	return node;
}

void parse_type(ParseState *ps) {
	if(is(ps, KW_VOID)) {
		next(ps);
	} else if(is(ps, KW_INT)) {
		next(ps);
	} else {
		c_error("%s is not a type at line %i", ps->token->string, ps->token->line);
	}
}

ASTNode *parse_identifier(ParseState *ps) {
	ASTNode *node = new_node(AST_IDENTIFIER);
	if(is(ps, IDENTIFIER)) {
		node->token = ps->token;
		next(ps);
		return node;
	} else {
		c_error("%s is not an IDENTIFIER at line %i", ps->token->string, ps->token->line);
	}
}

ASTNode *parse_literal(ParseState *ps) {
	ASTNode *node = new_node(AST_LITERAL);
	if(is(ps, NUMBER) || is(ps, STRING)) {
		node->token = ps->token;
		next(ps);
		return node;
	} else {
		c_error("%s is not an LITERAL at line %i", ps->token->string, ps->token->line);
	}
}

ASTNode *parse_decl(ParseState *ps) {
	if(is(ps, KW_STRING)) {
		ASTNode *node = new_node(AST_DECLARATOR);
		next(ps);
		node->left = parse_identifier(ps);
		if(!is(ps, SEMICOLON)) {
			expect(ps, ASSIGN);
			next(ps);
			node->right = parse_literal(ps);
			expect(ps, SEMICOLON);
			next(ps);
			return node;
		} else {
			next(ps);
			return node;
		}
	} else {
		c_error("Unknown type %s at line %i", ps->token->string, ps->token->line);
	}
}

ASTNode *parse_block(ParseState *ps) {
	ASTNode *node = new_node(AST_BLOCK);
	ASTNode node_body_head = {};
	node->body = &node_body_head;
	expect(ps, LBRACE);
	next(ps);
	while(!is(ps, RBRACE)) {
		node->body->next = parse_stmt(ps);
		node->body = node->body->next;
	}
	expect(ps, RBRACE);
	next(ps);
	node->body = node_body_head.next;
	return node;
}

ASTNode *parse_call(ParseState *ps) {
	ASTNode *node = new_node(AST_CALL);
	ASTNode node_body_head = {};
	node->body = &node_body_head;
	node->identifier = parse_identifier(ps);
	expect(ps, LPAREN);
	next(ps);
	while(!is(ps, RPAREN)) {
		node->body->next = parse_identifier(ps);
		node->body = node->body->next;
	}
	expect(ps, RPAREN);
	next(ps);
	expect(ps, SEMICOLON);
	next(ps);
	node->arguments = node_body_head.next;
	return node;
}

ASTNode *parse_stmt(ParseState *ps) {
	switch(get_type(ps)) {
		case LBRACE:
		{
			return parse_block(ps);
		}
		break;
		case KW_STRING:
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
			c_error("%s is not a valid statement at line %i", ps->token->string, ps->token->line);
		}
		break;
	}
}

ASTNode* parse_function(ParseState *ps) {
	ASTNode *node = new_node(AST_FUNCTION);
	ASTNode node_body_head = {};
	node->body = &node_body_head;
	parse_type(ps);
	parse_identifier(ps);
	expect(ps, LPAREN);
	next(ps);
	expect(ps, RPAREN);
	next(ps);
	expect(ps, LBRACE);
	next(ps);
	while(!is(ps, RBRACE)) {
		node->body->next = parse_stmt(ps);
		node->body = node->body->next;
	}
	expect(ps, RBRACE);
	node->body = node_body_head.next;
	return node;
}

ASTNode *parse(Token *token) {
	
	Token *token_copy = token;
	while(token_copy != NULL) {
		if(token_copy->type != END_OF_TOKEN) {
			printf("Line: %i Type: %i Token: %s\n", token_copy->line, (int)token_copy->type, token_copy->string);
		}
		token_copy = token_copy->next;
	}
	
	ParseState ps = {};
	ps.token = token;

	ASTNode *node = parse_function(&ps);
	return node;
}