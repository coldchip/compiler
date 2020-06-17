#include <stdio.h>
#include <stdlib.h>
#include "chipcode.h"

void emit(OpCode op, Register a, Register b, int c, int d) {
	switch(op) {
		case OP_MOV:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("mov $%i, $%i\n", c, d);
			} else {
				printf("mov %i, %i\n", a, b);
			}
		}
		break;
		case OP_JMP:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("jmp $%i, $%i\n", c, d);
			} else {
				printf("jmp %i, %i\n", a, b);
			}
		}
		break;
		case OP_SUB:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("sub $%i, $%i\n", c, d);
			} else {
				printf("sub %i, %i\n", a, b);
			}
		}
		break;
		case OP_DIV:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("div $%i, $%i\n", c, d);
			} else {
				printf("div %i, %i\n", a, b);
			}
		}
		break;
		case OP_MUL:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("mul $%i, $%i\n", c, d);
			} else {
				printf("mul %i, %i\n", a, b);
			}
		}
		break;
		case OP_ADD:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("add $%i, $%i\n", c, d);
			} else {
				printf("add %i, %i\n", a, b);
			}
		}
		break;
		case OP_CALL:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("call $%i, $%i\n", c, d);
			} else {
				printf("call %i, %i\n", a, b);
			}
		}
		break;
		case OP_CMP:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("cmp $%i, $%i\n", c, d);
			} else {
				printf("cmp %i, %i\n", a, b);
			}
		}
		break;
		case OP_PUSH:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("push $%i, $%i\n", c, d);
			} else {
				printf("push %i, %i\n", a, b);
			}
		}
		break;
		case OP_POP:
		{
			if(a == NO_REG && b == NO_REG) {
				printf("pop $%i, $%i\n", c, d);
			} else {
				printf("pop %i, %i\n", a, b);
			}
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
	emit(OP_PUSH, NO_REG, NO_REG, 0, 0);
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	emit(OP_POP, NO_REG, NO_REG, 0, 0);
	node_free(node);
}

void enter_block(Node *node) {
	printf(".block\n");
	emit(OP_PUSH, NO_REG, NO_REG, 0, 0);
	ListEntry *entry = node->bodylist->start;
	while(entry != NULL) {
		visitor(entry->ptr);
		entry = entry->next;
	}
	emit(OP_POP, NO_REG, NO_REG, 0, 0);
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
	emit(OP_POP, R0, NO_REG, 0, 0);
	emit(OP_POP, R1, NO_REG, 0, 0);
	emit(OP_ADD, R0, R1, 0, 0);
	// Push last result to stack
	emit(OP_PUSH, R0, NO_REG, 0, 0);
	node_free(node);
}

void enter_literal(Node *node) {
	emit(OP_PUSH, NO_REG, NO_REG, atoi(node->token->data), 0);
	node_free(node);
}

void enter_ident(Node *node) {
	emit(OP_PUSH, NO_REG, NO_REG, atoi(node->token->data), 0);
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
		default:
		{
			
		}
		break;
	}
}

void generate(Node *node) {
	visitor(node);
}