#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include "chipcode.h"

static void c_error(char *format, ...) {
	va_list args;
    va_start(args, format);

	char fmt[1000];
	snprintf(fmt, sizeof(fmt), "[GEN] %s\n", format);
	vprintf(fmt, args);
    
    va_end(args);
	exit(1);
}

NodeType get_node_type(ASTNode *node) {
	return node->type;
}

void stack_inc(GenState *gs, int inc) {
	gs->sp += inc;
}

int indent = 0;

void emit(char *format, ...) {
	char indent_data[1000];
	indent_data[0] = '\0';
	for(int i = 0; i < indent; i++) {
		strcat(indent_data, "     ");
	}

	va_list args;
    va_start(args, format);

	char fmt[1000];
	snprintf(fmt, sizeof(fmt), "%s%s", indent_data, format);
	vprintf(fmt, args);
    
    va_end(args);
}

void enter_argument(GenState *gs, ASTNode *node) {
	ASTNode *params = node->body;
	int param_count = 0;
	while(params != NULL) {
		char *ident = (char*)visitor(gs, params);
		emit("acop s%i r%i\n", gs->sp, param_count);
		ir_add_instruction(gs->ir, ACOP);
		symtable_add(gs->st, ident, gs->sp);
		stack_inc(gs, 1);
		symtable_add(gs->st, ident, 0);
		params = params->next;
		param_count++;
	}
}

void enter_parameter(GenState *gs, ASTNode *node) {
	ASTNode *params = node->body;
	int param_count = 0;
	while(params != NULL) {
		if(get_node_type(params) == AST_IDENTIFIER) {
			char *ident = (char*)visitor(gs, params);
			emit("acop r%i s%i\n", param_count, symtable_ptr(gs->st, ident));
			ir_add_instruction(gs->ir, ACOP);	
		}
		if(get_node_type(params) == AST_LITERAL) {
			char *literal = (char*)visitor(gs, params);
			emit("imov r%i \"%s\"\n", param_count, literal);
			ir_add_instruction(gs->ir, IMOV);	
		}
		params = params->next;
		param_count++;
	}
}

void enter_program(GenState *gs, ASTNode *node) {
	emit("program {\n");
	indent++;
	if(node->body != NULL) {
		visitor(gs, node->body);
	}
	indent--;
	emit("}\n");
}

void enter_function(GenState *gs, ASTNode *node) {


	char *func_name = (char*)visitor(gs, node->identifier);

	SymbolTable *st_orig  = gs->st;
	gs->st = symtable_clone(gs->st);
	gs->sp = 0;
	
	emit("function %s {\n", func_name);
	indent++;

	visitor(gs, node->args);

	if(node->body != NULL) {
		visitor(gs, node->body);
	}

	indent--;
	emit("}\n");

	symtable_free(gs->st);
	gs->st = st_orig;
}

void enter_block(GenState *gs, ASTNode *node) {
	SymbolTable *st_orig  = gs->st;
	gs->st = symtable_clone(gs->st);

	if(node->body != NULL) {
		visitor(gs, node->body);
	}

	symtable_free(gs->st);
	gs->st = st_orig;
}

void enter_declarator(GenState *gs, ASTNode *node) {
	if(node->left != NULL) {
		char *ident = (char*)visitor(gs, node->left);
		if(node->right != NULL) {
			ASTNode *right = node->right;
			if(get_node_type(right) == AST_LITERAL) {
				// x = "Test"; x = 9;
				emit("imov s%i \"%s\"\n", gs->sp, (char*)visitor(gs, right));
				ir_add_instruction(gs->ir, IMOV);
				symtable_add(gs->st, ident, gs->sp);
				stack_inc(gs, 1);
			} else if(get_node_type(right) == AST_IDENTIFIER) {
				// x = y;
				symtable_add(gs->st, ident, symtable_ptr(gs->st, visitor(gs, right)));
				
			} else {
				c_error("Right Hand error");
			}
		}
	}
}

char *get_identifier(GenState *gs, ASTNode *node) {
	return node->token->string;
}

char *get_literal(GenState *gs, ASTNode *node) {
	return node->token->string;
}

void enter_call(GenState *gs, ASTNode *node) {
	char *ident = (char*)visitor(gs, node->identifier);
	visitor(gs, node->args);
	emit("call %s\n", ident);
	ir_add_instruction(gs->ir, CALL);

}

void *visitor(GenState *gs, ASTNode *node) {
	while(node != NULL) {
		switch(node->type) {
			case AST_PROGRAM:
			{
				enter_program(gs, node);
			}
			break;
			case AST_FUNCTION:
			{
				enter_function(gs, node);
			}
			break;
			case AST_ARGUMENT:
			{
				enter_argument(gs, node);
			}
			break;
			case AST_PARAMETER:
			{
				enter_parameter(gs, node);
			}
			break;
			case AST_BLOCK:
			{
				enter_block(gs, node);
			}
			break;
			case AST_DECLARATOR:
			{
				enter_declarator(gs, node);
			}
			break;
			case AST_CALL:
			{
				enter_call(gs, node);
			}
			break;
			case AST_IDENTIFIER:
			{
				return get_identifier(gs, node);
			}
			break;
			case AST_LITERAL:
			{
				return get_literal(gs, node);
			}
			break;
			default:
			{
				printf("Unknown Type: %i\n", node->type);
			}
			break;
		}
		node = node->next;
	}
	return NULL;
}

void generate(ASTNode *node) {
	GenState gs;
	gs.sp = 0;
	gs.st = symtable_init();
	gs.ir = ir_init();
	visitor(&gs, node);

	InstructionEntry *cs = gs.ir->start;

	while(cs != NULL) {
		switch(cs->op) {
			case NOP:
			{
				printf("NOP\n");
			}
			break;
			case ACOP:
			{
				printf("ACOP\n");
			}
			break;
			case IMOV:
			{
				printf("IMOV\n");
			}
			break;
			case CALL:
			{
				printf("CALL\n");
			}
			break;
		}
		cs = cs->next;
	}
}