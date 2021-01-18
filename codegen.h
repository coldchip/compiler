#ifndef CODEGEN_H
#define CODEGEN_H

#include "parse.h"

typedef struct _Generator {
	FILE *file;
} Generator;

void emit(Generator *generator, const char *op, const char *a, const char *b);
void emit_procedure(Generator *generator, const char *label);
void emit_param(Generator *generator, const char *label);

void gen_store(Generator *generator);
void gen_addr(Generator *generator, Node *node);

void enter_assign(Generator *generator, Node *node);
void enter_program(Generator *generator, Node *node);
void enter_function(Generator *generator, Node *node);
void enter_block(Generator *generator, Node *node);
void enter_decl(Generator *generator, Node *node);
void enter_binexpr(Generator *generator, Node *node);
void enter_literal(Generator *generator, Node *node);
void enter_ident(Generator *generator, Node *node);
void enter_call(Generator *generator, Node *node);
void enter_if(Generator *generator, Node *node);
void enter_while(Generator *generator, Node *node);
void enter_param(Generator *generator, Node *node);
void enter_arg(Generator *generator, Node *node);
void enter_return(Generator *generator, Node *node);
void visitor(Generator *generator, Node *node);
void generate(Node *node);

#endif