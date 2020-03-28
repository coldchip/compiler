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

int indent = 0;

void emit(char *format, ...) {
	char indent_data[1000];
	indent_data[0] = '\0';
	for(int i = 0; i < indent; i++) {
		strcpy(indent_data, "     ");
	}

	va_list args;
    va_start(args, format);

	char fmt[1000];
	snprintf(fmt, sizeof(fmt), "%s%s\n", indent_data, format);
	vprintf(fmt, args);
    
    va_end(args);
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
	emit("func %s", node->identifier->token->string);
	indent++;
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
		//emit("DECLARATOR NAME %s", ident);
	}
	if(node->right != NULL) {
		ASTNode *right = node->right;
		if(right->token->type == STRING) {
			// Char = 8Bytes
			emit("iloadc \"%s\"", right->token->string);
			for(int i = 0; i < strlen(right->token->string); i++) {
				gs->sp += 8;
			}
			printf("%i\n", gs->sp);
		} 
		if(right->token->type == NUMBER) {
			// Int = 32Bytes
			int number = atoi(right->token->string);
			emit("iloadn %i", number);
			gs->sp += 32;
		} else {

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
	emit("call %s", ident);
	emit("pusharg");
	emit("exec");
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