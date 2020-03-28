/*
	ColdChip inc. 
	@Author Ryan
*/

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
	snprintf(fmt, sizeof(fmt), "[LEXER] %s\n", format);
	vprintf(fmt, args);
    
    va_end(args);
	exit(1);
}

static Token *new_token(Token *next, TokenType type, char* string) {
	// Updates previous entry->next pointer with the malloc'd entry pointer
	Token *tok = malloc(sizeof(Token));
	memset(tok, 0, sizeof(Token));
	tok->string = string;
	tok->line = next->line;
	tok->type = type;
	tok->next = NULL;
	next->next = tok;
	return tok;
}

void expect_something(char *bit) {
	if(*bit == '\0') {
		c_error("Unexpected EOF reached");
	}
}

static bool startswith(char *p, char *q) {
	return strncmp(p, q, strlen(q)) == 0;
}

static bool is_keyword(char bit) {
	return ('a' <= bit && bit <= 'z') || ('A' <= bit && bit <= 'Z') || bit == '_';
}

static bool is_number(char bit) {
	return (bit >= '0' && bit <= '9');
}

static bool is_space(char bit) {
	return (bit == '	' || bit == ' ' || bit == 0x0d || bit == 0x0a);
}

bool strmatch(char *a, char *b) {
	return strcmp(a, b) == 0;
}

TokenType keyword_type(char *keyword) {
	if(strmatch(keyword, "void")) {
		return KW_VOID;
	} else if(strmatch(keyword, "var")) {
		return KW_VAR;
	} else {
		return IDENTIFIER;
	}
}

char *malloc_string_to_bit(char *data) {
	char *t = malloc(2);
	memcpy(t, data, 1);
	*(t + 1) = '\0';
	return t;
}

char *malloc_strcpy(char *data, int size) {
	char *keyword = malloc(size + 1);
	memcpy(keyword, data, size);
	*(keyword + size) = '\0';
	return keyword;
}

Token *lex(char *data) {
	Token token;
	Token *token_tracker = &token;
	memset(token_tracker, 0, sizeof(token));
	token_tracker->line = 1;

	while(*data != '\0') {
		if(is_keyword(*data)) {
			char *start = data;
			while(is_keyword(*data)) {
				data++;
			}
			char *keyword = malloc_strcpy(start, data - start);
			token_tracker = new_token(token_tracker, keyword_type(keyword), keyword);
			continue;
		} else if(is_number(*data)) {
			char *start = data;
			while(is_number(*data)) {
				data++;
			}
			char *keyword = malloc_strcpy(start, data - start);
			token_tracker = new_token(token_tracker, NUMBER, keyword);
			continue;
		} else if(*data == '"') {
			data++;
			expect_something(data);
			char *start = data;
			for(;;) {
				if(*data == '\0') {
					c_error("Unclosed string literal");
					break;
				} else if(*data == '\\') {
					data++;
				} else if(*data == '"') {
					break;
				}
				data++;
			}
			char *keyword = malloc_strcpy(start, data - start);
			token_tracker = new_token(token_tracker, STRING, keyword);
			data++;
			continue;
		} else if(*data == '\'') {
			data++;
			expect_something(data);
			char *start = data;
			for(;;) {
				if(*data == '\0') {
					c_error("Unclosed char literal");
					break;
				} else if(*data == '\\') {
					data++;
				} else if(*data == '\'') {
					break;
				}
				data++;
			}
			char *keyword = malloc_strcpy(start, data - start);
			token_tracker = new_token(token_tracker, STRING, keyword);
			data++;
			continue;
		} else if (startswith(data, "//")) {
			data += 2;
			while(*data != '\n' && *data != '\0') {
				data++;
			}
			continue;
		} else if(startswith(data, "/*")) {
			char *q = strstr(data + 2, "*/");
			if (!q) {
				c_error("unclosed block comment");
			}
			data = q + 2;
			continue;
		} else if(is_space(*data)) {
			if(*data == '\n') {
				token_tracker->line++;
			}
			data++;
			continue;
		} else {
			char *t = malloc_string_to_bit(data);
			switch(*data) {
				case '(':
				{
					token_tracker = new_token(token_tracker, LPAREN, t);
				}
				break;
				case ')':
				{
					token_tracker = new_token(token_tracker, RPAREN, t);
				}
				break;
				case '{':
				{
					token_tracker = new_token(token_tracker, LBRACE, t);
				}
				break;
				case '}':
				{
					token_tracker = new_token(token_tracker, RBRACE, t);
				}
				break;
				case ';':
				{
					token_tracker = new_token(token_tracker, SEMICOLON, t);
				}
				break;
				case ',':
				{
					token_tracker = new_token(token_tracker, COMMA, t);
				}
				break;
				case '+':
				{
					token_tracker = new_token(token_tracker, PLUS, t);
				}
				break;
				case '-':
				{
					token_tracker = new_token(token_tracker, MINUS, t);
				}
				break;
				case '*':
				{
					token_tracker = new_token(token_tracker, STAR, t);
				}
				break;
				case '/':
				{
					token_tracker = new_token(token_tracker, DIVIDE, t);
				}
				break;
				case '=':
				{
					token_tracker = new_token(token_tracker, ASSIGN, t);
				}
				break;
				case '<':
				{
					token_tracker = new_token(token_tracker, LESST, t);
				}
				break;
				case '>':
				{
					token_tracker = new_token(token_tracker, MORET, t);
				}
				break;
				default:
				{
					c_error("Unknown token %s at line %i", t, token_tracker->line);
					free(t);
				}
				break;
			}
			data++;
			continue;
		}
	}
	token_tracker = new_token(token_tracker, END_OF_TOKEN, "");
	return token.next;	
}	