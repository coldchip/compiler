#ifndef CHIPCODE_H
#define CHIPCODE_H

// lex.c
typedef enum {
	END_OF_TOKEN,
	IDENTIFIER,
	STRING,
	NUMBER,
	KW_VAR,
	KW_VOID,
	LPAREN,
	RPAREN,
	LBRACKET,
	RBRACKET,
	LBRACE,
	RBRACE,
	SEMICOLON,
	ASSIGN,
	PLUS,
	MINUS,
	STAR,
	DIVIDE,
	LESST,
	MORET
} TokenType;

typedef struct _Token {
	char *string;
	int line;
	TokenType type;
	struct _Token *next;
} Token;

char *malloc_string_to_bit(char *data);
char *malloc_strcpy(char *data, int size);
extern Token *lex(char *file);

// parse.c
typedef enum {
	AST_FUNCTION,
	AST_BIN_EXPR,
	AST_DECLARATOR,
	AST_IDENTIFIER,
	AST_LITERAL,
	AST_BLOCK,
	AST_IF,
	KW_ELSE,
	AST_WHILE,
	AST_CALL
} NodeType;

typedef struct _ASTNode {
	NodeType type;
	Token *token;
	struct _ASTNode *identifier;
	struct _ASTNode *condition;
	struct _ASTNode *body;
	struct _ASTNode *alternate;
	struct _ASTNode *left;
	TokenType op;
	struct _ASTNode *right;
	struct _ASTNode *arguments;
	struct _ASTNode *next;
} ASTNode;

typedef struct _ParseState {
	Token *token;
} ParseState;

ASTNode *parse(Token *token);
ASTNode *parse_stmt(ParseState *ps);

// generate.c

char text_region[10000];

char code_region[10000];

void *generate(ASTNode *node);

#endif