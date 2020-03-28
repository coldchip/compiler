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

void new_st(GenState *gs, char *name, int pointer) {
	SymbolTable *prev = gs->st;

	SymbolTable *next = malloc(sizeof(SymbolTable));
	memset(next, 0, sizeof(SymbolTable));

	char *name_malloc = malloc((sizeof(char) * strlen(name)) + 1);
	strcpy(name_malloc, name);

	next->name = name_malloc;
	next->pointer = pointer;
	if(prev->start == NULL) {
		prev->start = next;
	}
	next->start = prev->start;

	prev->next = next;

	gs->st = next;
}

bool has_var(GenState *gs, char *name) {
	SymbolTable *current = gs->st->start;
	while(current != NULL) {
		if(strcmp(name, current->name) == 0) {
			return true;
		}
		current = current->next;
	}
}

int get_pointer(GenState *gs, char *name) {
	SymbolTable *current = gs->st->start;
	while(current != NULL) {
		if(strcmp(name, current->name) == 0) {
			return current->pointer;
		}
		current = current->next;
	}
	return 0;
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
	while(params != NULL) {
		char *ident = (char*)generate(gs, params);
		emit("iload %i", get_pointer(gs, ident));
		params = params->next;
	}
}

void enter_param(GenState *gs, ASTNode *node) {
	ASTNode *params = node->body;
	while(params != NULL) {
		char *ident = (char*)generate(gs, params);
		if(params->type == AST_LITERAL) {
			emit("istorec \"%s\"", ident);
			params = params->next;
		} else {
			if(has_var(gs, ident)) {
				emit("istore %i", get_pointer(gs, ident));
				params = params->next;
			} else {
				c_error("Undefined parameter variable \"%s\"", ident);
			}
		}
	}
}

void enter_program(GenState *gs, ASTNode *node) {
	emit("program");
	indent++;
	if(node->body != NULL) {
		generate(gs, node->body);
	}
	indent--;
}

void enter_function(GenState *gs, ASTNode *node) {
	char *func_name = (char*)generate(gs, node->identifier);
	if(has_var(gs, func_name)) {
		c_error("Duplicate of function \"%s\"", func_name);
	}
	new_st(gs, func_name, 0);
	emit("func %s", node->identifier->token->string);
	indent++;
	generate(gs, node->args);
	if(node->body != NULL) {
		generate(gs, node->body);
	}
	indent--;
}

void enter_block(GenState *gs, ASTNode *node) {
	emit("pushstack");
	if(node->body != NULL) {
		generate(gs, node->body);
	}
	emit("popstack");
}

void enter_declarator(GenState *gs, ASTNode *node) {
	if(node->left != NULL) {
		char *ident = (char*)generate(gs, node->left);
		if(has_var(gs, ident)) {
			c_error("Error, Variable %s already existed. ", ident);
		}
		if(node->right != NULL) {
			ASTNode *right = node->right;
			if(right->token->type == STRING) {
				emit("iloadc \"%s\"", right->token->string);
				new_st(gs, ident, gs->sp);
				for(int i = 0; i < strlen(right->token->string); i++) {
					stack_inc(gs, 8);
				}
			} 
			if(right->token->type == NUMBER) {
				int number = atoi(right->token->string);
				emit("iloadn %i", number);
				new_st(gs, ident, gs->sp);
				stack_inc(gs, 32);
			} else {

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
	char *ident = (char*)generate(gs, node->identifier);
	if(has_var(gs, ident)) {
		emit("call %s", ident);
		generate(gs, node->args);
		emit("pusharg");
		emit("exec");
	} else {
		c_error("Call to undefined function \"%s\"", ident);
	}

}

void *generate(GenState *gs, ASTNode *node) {
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
			case AST_PARAM:
			{
				enter_param(gs, node);
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