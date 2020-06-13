#include <stdio.h>
#include <stdlib.h>
#include "chipcode.h"

void enter_program(Node *node) {
	printf(".program\n");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	node_free(node);
}

void enter_function(Node *node) {
	printf(".function\n");
	printf("push\n");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	printf("pop\n");
	node_free(node);
}

void enter_decl(Node *node) {
	visitor(node->body);
	node_free(node);
}

void enter_binexpr(Node *node) {
	// Enter deepest first
	if(node->right) {
		visitor(node->right);
	}
	if(node->left) {
		visitor(node->left);
	}
	printf("pop r0\n");
	printf("pop r1\n");
	printf("add r0 r1\n");
	// Push last result to stack
	printf("push r0\n");
	node_free(node);
}

void enter_literal(Node *node) {
	printf("push %s\n", node->token->data);
	node_free(node);
}

void enter_ident(Node *node) {
	printf("push $%s\n", node->token->data);
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
		case AST_DECL:
		{
			enter_decl(node);
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