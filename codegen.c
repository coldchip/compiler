#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "chipcode.h"

int indent = 0;

void emit(char *format, ...) {
	va_list args;
    va_start(args, format);

	char fmt[1000];

	for(int i = 0; i < indent; i++) {
		printf("    ");
	}

	snprintf(fmt, sizeof(fmt), "%s\n", format);

	vprintf(fmt, args);
    
    va_end(args);
}

void enter_program(Node *node) {
	emit(".program");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	node_free(node);
}

void enter_function(Node *node) {
	emit("%s:", node->token->data);
	indent++;
	emit("push rsp");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	emit("pop rsp");
	indent--;
	node_free(node);
}

void enter_block(Node *node) {
	//emit(".block");
	//emit("push");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	//emit("pop");
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
	emit("pop r0");
	emit("pop r1");
	emit("add r0, r1");
	// Push last result to stack
	emit("push r0");
	node_free(node);
}

void enter_literal(Node *node) {
	emit("push %i", atoi(node->token->data));
	node_free(node);
}

void enter_ident(Node *node) {
	emit("push %i", atoi(node->token->data));
	node_free(node);
}

void enter_call(Node *node) {
	emit("call %s", node->token->data);
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
			enter_block(node);
		}
		break;
		case AST_CALL:
		{
			enter_call(node);
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