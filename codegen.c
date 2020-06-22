#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "chipcode.h"

void enter_program(Generator *generator, Node *node) {
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(generator, entry->ptr);
		entry = entry->next;
	}
	node_free(node);
}

void enter_function(Generator *generator, Node *node) {
	op_exec(generator->process, "push", "fp", NULL);
	op_exec(generator->process, "mov", "fp", "sp");
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(generator, entry->ptr);
		entry = entry->next;
	}
	op_exec(generator->process, "mov", "sp", "fp");
	op_exec(generator->process, "pop", "fp", NULL);
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
	op_exec(generator->process, "pop", "r0", NULL);
	op_exec(generator->process, "pop", "r1", NULL);
	switch(node->type) {
		case AST_ADD:
		{
			op_exec(generator->process, "add", "r0", "r1");
		}
		break;
		case AST_SUB:
		{
			op_exec(generator->process, "sub", "r0", "r1");
		}
		break;
		case AST_MUL:
		{
			op_exec(generator->process, "mul", "r0", "r1");
		}
		break;
		case AST_DIV:
		{
			op_exec(generator->process, "div", "r0", "r1");
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
	op_exec(generator->process, "push", "r0", NULL);
	node_free(node);
}

void enter_literal(Generator *generator, Node *node) {
	op_exec(generator->process, "push", (node->token->data), NULL);
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	char offset_data[1000];
	sprintf(offset_data, "fp+%li", node->offset);
	op_exec(generator->process, "lea", "r0", offset_data);
	op_exec(generator->process, "push", "r0", NULL);
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
	generator.process = new_process();

	visitor(&generator, node);

	print_hex(generator.process->stack);

	printf("Result Yield: %lu\n", generator.process->r0);

	free_process(generator.process);
}