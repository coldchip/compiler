/*
	ColdChip inc. 
	@Author Ryan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "chipcode.h"

Token *new_token(Token *prev, TokenType type, char* data) {
	// Updates previous entry->next pointer with the malloc'd entry pointer
	Token *tok = malloc(sizeof(Token));
	memset(tok, 0, sizeof(Token));
	tok->data = data;
	tok->line = prev->line;
	tok->type = type;
	tok->next = NULL;
	prev->next = tok;
	return tok;
}

void assert_not_eof(char *bit) {
	if(*bit == '\0') {
		c_error("Unexpected EOF reached");
	}
}

bool startswith(char *p, char *q) {
	return strncmp(p, q, strlen(q)) == 0;
}

bool is_keyword(char bit) {
	return ('a' <= bit && bit <= 'z') || ('A' <= bit && bit <= 'Z') || bit == '_';
}

bool is_number(char bit) {
	return (bit >= '0' && bit <= '9');
}

bool is_space(char bit) {
	return (bit == '	' || bit == ' ' || bit == 0x0d || bit == 0x0a);
}

bool strmatch(char *a, char *b) {
	return strcmp(a, b) == 0;
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
			token_tracker = new_token(token_tracker, TK_IDENT, keyword);
			continue;
		} else if(is_number(*data)) {
			char *start = data;
			while(is_number(*data)) {
				data++;
			}
			char *keyword = malloc_strcpy(start, data - start);
			token_tracker = new_token(token_tracker, TK_NUMBER, keyword);
			continue;
		} else if(*data == '"') {
			data++;
			assert_not_eof(data);
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
			token_tracker = new_token(token_tracker, TK_STRING, keyword);
			data++;
			continue;
		} else if(*data == '\'') {
			data++;
			assert_not_eof(data);
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
			token_tracker = new_token(token_tracker, TK_STRING, keyword);
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
		} else if(startswith(data, "==")) {
			char *t = malloc_strcpy(data, 2);
			token_tracker = new_token(token_tracker, TK_SPECIAL, t);
			data += 2;
			continue;
		} else if(is_space(*data)) {
			if(*data == '\n') {
				token_tracker->line++;
			}
			data++;
			continue;
		} else {
			char *t = malloc_string_to_bit(data);
			token_tracker = new_token(token_tracker, TK_SPECIAL, t);
			data++;
			continue;
		}
	}
	token_tracker = new_token(token_tracker, TK_EOF, NULL);
	return token.next;	
}

void token_free(Token *token) {
	while(token != NULL) {
		Token *token_next = token->next;
		if(token->data) {
			free(token->data);
		}
		free(token);
		token = token_next;
	}
}