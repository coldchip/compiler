#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>

#define VERSION 1.0
#define API extern

// chipcode.c

API void c_error(char *format, ...);

// macro.c

API char *macro(char *input);

// lex.c
typedef enum {
	TK_IDENT,
	TK_NUMBER,
	TK_STRING,
	TK_SPECIAL,
	TK_EOF
} TokenType;

typedef struct _Token {
	char *data;
	int line;
	TokenType type;
	struct _Token *next;
} Token;

char *malloc_string_to_bit(char *data);
char *malloc_strcpy(char *data, int size);
void assert_not_eof(char *bit);
Token *new_token(Token *prev, TokenType type, char* data);
bool is_space(char bit);
bool startswith(char *p, char *q);
bool is_keyword(char bit);
bool is_number(char bit);
bool strmatch(char *a, char *b);
API Token *lex(char *file);

// parse.c

typedef struct _Parser {
	Token *token;
} Parser;

typedef struct _Node {
	
} Node;

void parse_function(Parser *parser);
void parse_program(Parser *parser);
void expect(Parser *parser, const char *str);
void next(Parser *parser);
bool is_type(Parser *parser, TokenType type);
bool is_data(Parser *parser, const char *str);
bool is_typename(Parser *parser);
API void parse(Token *token);

#endif