#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "parse.h"
#include "emit.h"

typedef struct _Generator {
	int var_select;
	FILE *file;
	Emit *emit;
} Generator;

void emit(Generator *generator, const char *format, ...);

void gen_store(Generator *generator, Node *node);

void enter_assign        (Generator *generator, Node *node);
void enter_program       (Generator *generator, Node *node);
void enter_function      (Generator *generator, Node *node);
void enter_block         (Generator *generator, Node *node);
void enter_decl          (Generator *generator, Node *node);
void enter_binexpr       (Generator *generator, Node *node);
void enter_literal       (Generator *generator, Node *node);
void enter_char_literal  (Generator *generator, Node *node);
void enter_ident         (Generator *generator, Node *node);
void enter_ident_member  (Generator *generator, Node *node);
void enter_call          (Generator *generator, Node *node);
void enter_if            (Generator *generator, Node *node);
void enter_while         (Generator *generator, Node *node);
void enter_param         (Generator *generator, Node *node);
void enter_arg           (Generator *generator, Node *node);
void enter_return        (Generator *generator, Node *node);
void enter_string_concat (Generator *generator, Node *node);
void enter_string_literal(Generator *generator, Node *node);
void enter_derefrence    (Generator *generator, Node *node);
void enter_refrence      (Generator *generator, Node *node);
void visitor             (Generator *generator, Node *node);
void generate            (Node *node);

#endif