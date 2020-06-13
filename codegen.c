#include <stdio.h>
#include <stdlib.h>
#include "chipcode.h"

void emit(OpCode op, Register a, Register b) {
	printf("%i %i, %i\n", op, a, b);
	switch(op) {
		case OP_MOV:
		{

		}
		break;
		case OP_JMP:
		{

		}
		break;
		case OP_SUB:
		{

		}
		break;
		case OP_DIV:
		{

		}
		break;
		case OP_MUL:
		{

		}
		break;
		case OP_ADD:
		{

		}
		break;
		case OP_CALL:
		{

		}
		break;
		case OP_CMP:
		{

		}
		break;
		case OP_PUSH:
		{

		}
		break;
		case OP_POP:
		{

		}
		break;
	}
}

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
	emit(OP_PUSH, NO_REG, NO_REG);
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	emit(OP_POP, NO_REG, NO_REG);
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
	emit(OP_POP, R0, NO_REG);
	emit(OP_POP, R1, NO_REG);
	emit(OP_ADD, R0, R1);
	// Push last result to stack
	emit(OP_PUSH, R0, NO_REG);
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