#include "codegen.h"

void emit(Generator *generator, const char *format, ...) {
	va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

void gen_store(Generator *generator) {
	
}

void gen_addr(Generator *generator, Node *node) {

	node_free(node);
}

void enter_assign(Generator *generator, Node *node) {
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		gen_addr(generator, node->left);
	}
	gen_store(generator);
	node_free(node);
}

void enter_program(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}
	node_free(node);
}

void enter_function(Generator *generator, Node *node) {
	emit(generator, "func %s\n", node->token->data);
	if(node->args) {
		visitor(generator, node->args);
	}
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}
	node_free(node);
}

void enter_block(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}
	node_free(node);
}

void enter_decl(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
		emit(generator, "\tstore %s\n", node->token->data);
	}
	node_free(node);
}

void enter_binexpr(Generator *generator, Node *node) {
	// Enter deepest first
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		visitor(generator, node->left);
	}
	switch(node->type) {
		case AST_ADD:
		{
			emit(generator, "\tadd\n");
		}
		break;
		case AST_SUB:
		{
			emit(generator, "\tsub\n");
		}
		break;
		case AST_MUL:
		{
			emit(generator, "\tmul\n");
		}
		break;
		case AST_DIV:
		{
			emit(generator, "\tdiv\n");
		}
		break;
		case AST_EQUAL:
		{
			// Compare r0 and r1
			emit(generator, "\tcmp\n");
		}
		break;
		default:
		{
			c_error("%i\n", node->type);
		}
		break;
	}
	// Push last result to stack
	node_free(node);
}

void enter_literal(Generator *generator, Node *node) {
	emit(generator, "\tpush %s\n", (node->token->data));
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	emit(generator, "\tload %s\n", node->token->data);
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {
	if(node->args) {
		visitor(generator, node->args);
	}
	emit(generator, "\tcall %s\n", node->token->data);
	node_free(node);
}

void enter_if(Generator *generator, Node *node) {
	if(node->condition) {
		visitor(generator, node->condition);
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	if(node->alternate) {
		visitor(generator, node->alternate);
	}
	node_free(node);
}

void enter_while(Generator *generator, Node *node) {
	if(node->condition) {
		visitor(generator, node->condition);
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	node_free(node);
}

void enter_param(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		node_free(entry);
	}
	node_free(node);
}

void enter_arg(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}
	node_free(node);
}

void enter_return(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	node_free(node);
}

void enter_string_concat(Generator *generator, Node *node) {
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		visitor(generator, node->left);
	}
	emit(generator, "\tstr_concat\n");
	node_free(node);
}

void enter_string(Generator *generator, Node *node) {
	emit(generator, "\tpush_str %s\n", node->token->data);
	node_free(node);
}

void visitor(Generator *generator, Node *node) {
	switch(node->type) {
		case AST_PROGRAM:
		{
			enter_program(generator, node);
		}
		break;
		case AST_FUNCTION:
		{
			enter_function(generator, node);
		}
		break;
		case AST_PARAM:
		{
			enter_param(generator, node);
		}
		break;
		case AST_ARG:
		{
			enter_arg(generator, node);
		}
		break;
		case AST_ASSIGN:
		{
			enter_assign(generator, node);
		}
		break;
		case AST_ADD:
		case AST_SUB:
		case AST_MUL:
		case AST_DIV:
		case AST_LT:
		case AST_EQUAL:
		{
			enter_binexpr(generator, node);
		}
		break;
		case AST_LITERAL:
		{
			enter_literal(generator, node);
		}
		break;
		case AST_IDENT:
		{
			enter_ident(generator, node);
		}
		break;
		case AST_DECL:
		{
			enter_decl(generator, node);
		}
		break;
		case AST_IF:
		{
			enter_if(generator, node);
		}
		break;
		case AST_WHILE:
		{
			enter_while(generator, node);
		}
		break;
		case AST_BLOCK:
		{
			enter_block(generator, node);
		}
		break;
		case AST_CALL:
		{
			enter_call(generator, node);
		}
		break;
		case AST_RETURN:
		{
			enter_return(generator, node);
		}
		break;
		case AST_STRING_CONCAT:
		{
			enter_string_concat(generator, node);
		}
		break;
		case AST_STRING:
		{
			enter_string(generator, node);
		}
		break;
		default:
		{
			
		}
		break;
	}
}

void generate(Node *node) {
	Generator generator;
	generator.file = stdout;
	visitor(&generator, node);
}
