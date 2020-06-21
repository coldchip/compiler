#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "chipcode.h"

int indent = 0;
int label = 0;

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
	emit("push fp");
	emit("mov fp, sp");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	emit("mov sp, fp");
	emit("pop fp");
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
	switch(node->type) {
		case AST_ADD:
		{
			emit("add r0, r1");
		}
		break;
		case AST_EQUAL:
		{
			emit("cmp r0, r1");
			emit("sete r0");
		}
		break;
		default:
		{
			c_error("%i\n", node->type);
		}
		break;
	}
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

void enter_if(Node *node) {
	if(node->condition) {
		visitor(node->condition);
	}
	emit("pop r0");
	emit("cmp r0, 1");
	emit("jne .proc%i", label);
	if(node->body) {
		visitor(node->body);
	}
	indent--;
	emit(".proc%i", label);
	indent++;
	label++;
	if(node->alternate) {
		visitor(node->alternate);
	}
	node_free(node);
}

void enter_while(Node *node) {
	if(node->condition) {
		visitor(node->condition);
	}
	emit("pop r0");
	emit("cmp r0, 1");
	emit("jne .proc%i", label);
	if(node->body) {
		visitor(node->body);
	}
	emit("jmp ADDR");
	indent--;
	emit(".proc%i", label);
	indent++;
	label++;
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
		case AST_ASSIGN:
		case AST_ADD:
		case AST_SUB:
		case AST_LT:
		case AST_EQUAL:
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
			enter_if(node);
		}
		break;
		case AST_WHILE:
		{
			enter_while(node);
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