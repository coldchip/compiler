#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "chipcode.h"

void emit(Generator *generator, const char *op, const char *a, const char *b) {
	if(op) {
		if(a && b) {
			fprintf(generator->file, "\t%s %s %s\n", op, a, b);
		} else if(a) {
			fprintf(generator->file, "\t%s %s\n", op, a);
		} else if(b) {
			fprintf(generator->file, "\t%s %s\n", op, b);
		} 
	}
}

void emit_label(Generator *generator, const char *label) {
	if(label) {
		fprintf(generator->file, "%s:\n", label);
	}
}

void emit_param(Generator *generator, const char *label) {
	if(label) {
		fprintf(generator->file, "%s\n", label);
	}
}

void enter_program(Generator *generator, Node *node) {
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(generator, entry->ptr);
		entry = entry->next;
	}
	node_free(node);
}

void enter_function(Generator *generator, Node *node) {
	emit_label(generator, node->token->data);
	emit(generator, "push", "fp", NULL);
	emit(generator, "mov", "fp", "sp");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(generator, entry->ptr);
		entry = entry->next;
	}
	emit(generator, "mov", "sp", "fp");
	emit(generator, "pop", "fp", NULL);
	node_free(node);
}

void enter_block(Generator *generator, Node *node) {
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(generator, entry->ptr);
		entry = entry->next;
	}
	node_free(node);
}

void enter_decl(Generator *generator, Node *node) {
	visitor(generator, node->body);
	emit(generator, "pop", "r0", NULL);
	emit(generator, "push", "r0", NULL);
	node_free(node);
}

void enter_binexpr(Generator *generator, Node *node) {
	// Enter deepest first
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		visitor(generator, node->left);
	}
	emit(generator, "pop", "r0", NULL);
	emit(generator, "pop", "r1", NULL);
	switch(node->type) {
		case AST_ADD:
		{
			emit(generator, "add", "r0", "r1");
		}
		break;
		case AST_SUB:
		{
			emit(generator, "sub", "r0", "r1");
		}
		break;
		case AST_MUL:
		{
			emit(generator, "mul", "r0", "r1");
		}
		break;
		case AST_DIV:
		{
			emit(generator, "div", "r0", "r1");
		}
		break;
		case AST_EQUAL:
		{

		}
		break;
		default:
		{
			c_error("%i\n", node->type);
		}
		break;
	}
	// Push last result to stack
	emit(generator, "push", "r0", NULL);
	node_free(node);
}

void enter_literal(Generator *generator, Node *node) {
	emit(generator, "push", (node->token->data), NULL);
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	char offset_data[1000];
	sprintf(offset_data, "[fp+%li]", node->offset);
	emit(generator, "lea", "r0", offset_data);
	emit(generator, "push", "r0", NULL);
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {
	node_free(node);
}

void enter_if(Generator *generator, Node *node) {
	if(node->condition) {
		visitor(generator, node->condition);
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	if(node->alternate) {
		visitor(generator, node->alternate);
	}
	node_free(node);
}

void enter_while(Generator *generator, Node *node) {
	if(node->condition) {
		visitor(generator, node->condition);
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	node_free(node);
}

void visitor(Generator *generator, Node *node) {
	switch(node->type) {
		case AST_PROGRAM:
		{
			enter_program(generator, node);
		}
		break;
		case AST_FUNCTION:
		{
			enter_function(generator, node);
		}
		break;
		case AST_ASSIGN:
		case AST_ADD:
		case AST_SUB:
		case AST_MUL:
		case AST_DIV:
		case AST_LT:
		case AST_EQUAL:
		{
			enter_binexpr(generator, node);
		}
		break;
		case AST_LITERAL:
		{
			enter_literal(generator, node);
		}
		break;
		case AST_IDENT:
		{
			enter_ident(generator, node);
		}
		break;
		case AST_DECL:
		{
			enter_decl(generator, node);
		}
		break;
		case AST_IF:
		{
			enter_if(generator, node);
		}
		break;
		case AST_WHILE:
		{
			enter_while(generator, node);
		}
		break;
		case AST_BLOCK:
		{
			enter_block(generator, node);
		}
		break;
		case AST_CALL:
		{
			enter_call(generator, node);
		}
		break;
		default:
		{
			
		}
		break;
	}
}

void generate(Node *node) {
	Generator generator;
	generator.file = fopen("bin/out.S", "wb");
	emit_param(&generator, "[asm]");
	visitor(&generator, node);
	fclose(generator.file);
}