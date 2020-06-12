#include <stdio.h>
#include <stdlib.h>
#include "chipcode.h"

void enter_program(Node *node) {
	printf(".program\n");
	ListEntry *entry = node->body->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	free(node->body);
	free(node);
}

void enter_function(Node *node) {
	printf(".function\n");
	ListEntry *entry = node->body->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	node_free(node);
}

void enter_binexpr(Node *node) {
	printf("Expr\n");
	if(node->left) {
		visitor(node->left);
	}
	if(node->right) {
		visitor(node->right);
	}
	node_free(node);
}

void enter_literal(Node *node) {
	printf("literal %s\n", node->token->data);
	node_free(node);
}

void enter_ident(Node *node) {
	printf("ident %s\n", node->token->data);
	node_free(node);
}

void visitor(Node *node) {
	switch(node->type) {
		case AST_PROGRAM:
		{
			enter_program(node);
		}
		break;
		case AST_FUNCTION:
		{
			enter_function(node);
		}
		break;
		case AST_BINEXPR:
		{
			enter_binexpr(node);
		}
		break;
		case AST_LITERAL:
		{
			enter_literal(node);
		}
		break;
		case AST_IDENT:
		{
			enter_ident(node);
		}
		break;
		case AST_IF:
		{

		}
		break;
		case AST_WHILE:
		{

		}
		break;
		case AST_BLOCK:
		{

		}
		break;
		default:
		{

		}
		break;
	}
}

void generate(Node *node) {
	visitor(node);
}