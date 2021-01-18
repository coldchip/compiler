#ifndef LEXX_H
#define LEXX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "chipcode.h"
#include "list.h"

typedef enum {
	TK_IDENT,
	TK_NUMBER,
	TK_STRING,
	TK_SPECIAL,
	TK_EOF
} TokenType;

typedef struct _Token {
	ListNode node;
	char *data;
	int line;
	TokenType type;
} Token;

char *malloc_string_to_bit(char *data);
char *malloc_strcpy(char *data, int size);
void assert_not_eof(char *bit);
Token *new_token(TokenType type, char* data, int line);
bool is_space(char bit);
bool startswith(char *p, char *q);
bool is_keyword(char bit);
bool is_number(char bit);
bool strmatch(char *a, char *b);
void lex(List *list, char *file);
void token_free(List *list);

#endif