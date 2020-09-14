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

void gen_store(Generator *generator) {
	emit(generator, "pop", "r0", NULL);
	emit(generator, "pop", "r1", NULL);
	emit(generator, "mov", "[r0], r1", NULL);
}

void gen_addr(Generator *generator, Node *node) {
	char offset_data[1000];
	sprintf(offset_data, "fp+%li", node->offset);
	emit(generator, "mov", "r0", offset_data);
	emit(generator, "push", "r0", NULL);
	node_free(node);
}

void enter_assign(Generator *generator, Node *node) {
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		gen_addr(generator, node->left);
	}
	gen_store(generator);
	node_free(node);
}

void enter_program(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}
	node_free(node);
}

void enter_function(Generator *generator, Node *node) {
	emit_label(generator, node->token->data);
	emit(generator, "push", "fp", NULL);
	emit(generator, "mov", "fp", "sp");
	if(node->args) {
		visitor(generator, node->args);
	}
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}
	emit(generator, "mov", "sp", "fp");
	emit(generator, "pop", "fp", NULL);
	node_free(node);
}

void enter_block(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}
	node_free(node);
}

void enter_decl(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
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
	sprintf(offset_data, "@fp+%li", node->offset);
	emit(generator, "mov", "r0", offset_data);
	emit(generator, "push", "r0", NULL);
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {
	if(node->args) {
		visitor(generator, node->args);
	}
	emit(generator, "call", node->token->data, NULL);
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

void enter_param(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	int ptr = 0;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		node_free(entry);
		
		char offset_data[1000];
		sprintf(offset_data, "@r5+%i", ptr);
		emit(generator, "mov", "r0", offset_data);
		emit(generator, "push", "r0", NULL);
		ptr += 8;
	}
	node_free(node);
}

void enter_arg(Generator *generator, Node *node) {
	emit(generator, "mov", "r5", "sp");
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
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
		case AST_PARAM:
		{
			enter_param(generator, node);
		}
		break;
		case AST_ARG:
		{
			enter_arg(generator, node);
		}
		break;
		case AST_ASSIGN:
		{
			enter_assign(generator, node);
		}
		break;
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