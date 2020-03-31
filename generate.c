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

static char *rand_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
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
		emit("iload reg%i", param_count);
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
			if(symtable_has(gs->st, ident)) {
				emit("istore reg%i %i", param_count, symtable_ptr(gs->st, ident));
			} else {
				c_error("Undefined parameter variable \"%s\"", ident);
			}
		}
		params = params->next;
		param_count++;
	}
}

void enter_program(GenState *gs, ASTNode *node) {
	emit("program");
	indent++;
	if(node->body != NULL) {
		visitor(gs, node->body);
	}
	indent--;
}

void enter_function(GenState *gs, ASTNode *node) {


	char *func_name = (char*)visitor(gs, node->identifier);
	if(symtable_has(gs->st, func_name)) {
		c_error("Duplicate of function \"%s\"", func_name);
	}
	symtable_add(gs->st, func_name, 0);
	emit("func %s", node->identifier->token->string);
	indent++;
	visitor(gs, node->args);
	if(node->body != NULL) {
		visitor(gs, node->body);
	}
	indent--;

}

void enter_block(GenState *gs, ASTNode *node) {
	emit("pushstack");
	if(node->body != NULL) {
		visitor(gs, node->body);
	}
	emit("popstack");
}

void enter_declarator(GenState *gs, ASTNode *node) {
	if(node->left != NULL) {
		char *ident = (char*)visitor(gs, node->left);
		if(symtable_has(gs->st, ident)) {
			c_error("Error, Variable %s already existed. ", ident);
		}
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
				if(symtable_has(gs->st, visitor(gs, right))) {
					symtable_add(gs->st, ident, symtable_ptr(gs->st, visitor(gs, right)));
				} else {
					c_error("Variable %s not defined", visitor(gs, right));
				}
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
	if(symtable_has(gs->st, ident) || strcmp(ident, "__call__") == 0) {
		visitor(gs, node->args);
		emit("call %s", ident);
	} else {
		c_error("Call to undefined function \"%s\"", ident);
	}

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
}

void *generate(GenState *gs, ASTNode *node) {
	gs->st = symtable_init();
	visitor(gs, node);
}















































































































void dbg_gen(ASTNode *node) {
	while(node != NULL) {
		switch(node->type) {
			case AST_FUNCTION:
			{
				printf("FUNCTION\n");
				if(node->body != NULL) {
					dbg_gen(node->body);
				}
			}
			break;
			case AST_BLOCK:
			{
				printf("BLOCK\n");
				if(node->body != NULL) {
					dbg_gen(node->body);
				}
			}
			break;
			case AST_DECLARATOR:
			{
				printf("DECLARATOR\n");
				if(node->left != NULL) {
					printf("DECLARATOR NAME\n");
					dbg_gen(node->left);
				}
				if(node->right != NULL) {
					printf("DECLARATOR body\n");
					dbg_gen(node->right);
				}
			}
			break;
			case AST_BIN_EXPR:
			{
				printf("BIN_EXPR\n");
				if(node->left != NULL) {
					printf("LEFT: ");
					dbg_gen(node->left);
				}
				if(node->right != NULL) {
					printf("RIGHT: ");
					dbg_gen(node->right);
				}
			}
			break;
			case AST_IF:
			{
				printf("IF\n");
				if(node->condition != NULL) {
					printf("CONDITION: \n");
					dbg_gen(node->condition);
				}
				if(node->body != NULL) {
					printf("CONSEQUENT: \n");
					dbg_gen(node->body);
				}
				if(node->alternate != NULL) {
					printf("ALTERNATIVE: \n");
					dbg_gen(node->alternate);
				}
			}
			break;
			case AST_WHILE:
			{
				printf("WHILE\n");
				if(node->condition != NULL) {
					printf("CONDITION: \n");
					dbg_gen(node->condition);
				}
				if(node->body != NULL) {
					printf("CONSEQUENT: \n");
					dbg_gen(node->body);
				}
			}
			break;
			case AST_IDENTIFIER:
			{
				printf("IDENTIFIER [%s]\n", node->token->string);
				
			}
			break;
			case AST_LITERAL:
			{
				printf("LITERAL [%s]\n", node->token->string);
				
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
}