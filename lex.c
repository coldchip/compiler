/*
	ColdChip inc. 
	@Author Ryan
*/
#include "lex.h"

Token *new_token(TokenType type, char* data, int line) {
	Token *token = malloc(sizeof(Token));
	token->data = data;
	token->type = type;
	token->line = line;
	return token;
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

void lex(List *tokens, char *data) {
	list_clear(tokens);

	int line = 1;

	while(*data != '\0') {
		if(is_keyword(*data)) {
			char *start = data;
			while(is_keyword(*data) || is_number(*data)) {
				data++;
			}
			char *keyword = malloc_strcpy(start, data - start);
			list_insert(list_end(tokens), new_token(TK_IDENT, keyword, line));
			continue;
		} else if(is_number(*data)) {
			char *start = data;
			while(is_number(*data)) {
				data++;
			}
			char *keyword = malloc_strcpy(start, data - start);
			list_insert(list_end(tokens), new_token(TK_NUMBER, keyword, line));
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
			list_insert(list_end(tokens), new_token(TK_STRING, keyword, line));
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
			list_insert(list_end(tokens), new_token(TK_STRING, keyword, line));
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
			list_insert(list_end(tokens), new_token(TK_SPECIAL, t, line));
			data += 2;
			continue;
		} else if(is_space(*data)) {
			if(*data == '\n') {
				line++;
			}
			data++;
			continue;
		} else {
			char *t = malloc_string_to_bit(data);
			list_insert(list_end(tokens), new_token(TK_SPECIAL, t, line));
			data++;
			continue;
		}
	}
	list_insert(list_end(tokens), new_token(TK_EOF, NULL, line));
}

void token_free(List *list) {
	while(!list_empty(list)) {
		Token *current = (Token*)list_remove(list_begin(list));
		if(current->data != NULL) {
			free(current->data);
		}
		free(current);
	}
}