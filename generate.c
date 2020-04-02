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
	snprintf(fmt, sizeof(fmt), "%s%s\n", indent_data, format);
	vprintf(fmt, args);
    
    va_end(args);
}

void enter_argument(GenState *gs, ASTNode *node) {
	ASTNode *params = node->body;
	int param_count = 0;
	while(params != NULL) {
		char *ident = (char*)visitor(gs, params);
		emit("args reg%i", param_count);
		symtable_add(gs->st, ident, 0);
		params = params->next;
		param_count++;
	}
}

void enter_parameter(GenState *gs, ASTNode *node) {
	ASTNode *params = node->body;
	int param_count = 0;
	while(params != NULL) {
		char *ident = (char*)visitor(gs, params);
		if(params->type == AST_LITERAL) {
			emit("istorec reg%i \"%s\"", param_count, ident);
		} else {
			emit("istore reg%i %i", param_count, symtable_ptr(gs->st, ident));		
		}
		params = params->next;
		param_count++;
	}
}

void enter_program(GenState *gs, ASTNode *node) {
	emit("program {");
	indent++;
	if(node->body != NULL) {
		visitor(gs, node->body);
	}
	indent--;
	emit("}");
}

void enter_function(GenState *gs, ASTNode *node) {


	char *func_name = (char*)visitor(gs, node->identifier);
	symtable_add(gs->st, func_name, 0);

	SymbolTable *st_orig  = gs->st;
	gs->st = symtable_clone(gs->st);
	
	emit("function %s {", node->identifier->token->string);
	indent++;
	visitor(gs, node->args);
	if(node->body != NULL) {
		visitor(gs, node->body);
	}
	indent--;
	emit("}");

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
				// x = "Test";
				emit("iloadc \"%s\"", visitor(gs, right));
				symtable_add(gs->st, ident, gs->sp);
				for(int i = 0; i < strlen(visitor(gs, right)); i++) {
					stack_inc(gs, 8);
				}
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
	emit("call %s", ident);
	

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
	gs.sp = 0x00;
	gs.st = symtable_init();
	visitor(&gs, node);
}