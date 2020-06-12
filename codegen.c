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

void enter_expr(Node *node) {
	printf("Expr\n");
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
		case AST_EXPR:
		{
			enter_expr(node);
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