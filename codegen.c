#include "codegen.h"

void emit(Generator *generator, const char *format, ...) {
	va_list args;
    va_start(args, format);
    vfprintf(generator->file, format, args);
    va_end(args);
}

void gen_store(Generator *generator, Node *node) {
	emit(generator, "\tstore %s\n", node->token->data);
	int i = emit_add_to_constant_pool(generator->emit, node->token->data);
	emit_opcode(generator->emit, BC_STORE, i, 0);
	node_free(node);
}

void enter_assign(Generator *generator, Node *node) {
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		gen_store(generator, node->left);
	}
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
	emit(generator, "@func %s\n", node->token->data);
	emit_select_function(generator->emit, node->token->data);
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
		int i = emit_add_to_constant_pool(generator->emit, node->token->data);
		emit_opcode(generator->emit, BC_STORE, i, 0);
	}
	node_free(node);
}

void enter_binexpr(Generator *generator, Node *node) {
	/* Enter deepest first */
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
			emit_opcode(generator->emit, BC_ADD, 0, 0);
		}
		break;
		case AST_SUB:
		{
			emit(generator, "\tsub\n");
			emit_opcode(generator->emit, BC_SUB, 0, 0);
		}
		break;
		case AST_MUL:
		{
			emit(generator, "\tmul\n");
			emit_opcode(generator->emit, BC_MUL, 0, 0);
		}
		break;
		case AST_DIV:
		{
			emit(generator, "\tdiv\n");
			emit_opcode(generator->emit, BC_DIV, 0, 0);
		}
		break;
		case AST_EQUAL:
		{
			emit(generator, "\tcmp\n");
		}
		break;
		case AST_GT:
		{
			emit(generator, "\tcmpgt\n");
			emit_opcode(generator->emit, BC_CMPGT, 0, 0);
		}
		break;
		case AST_LT:
		{
			emit(generator, "\tcmplt\n");
			emit_opcode(generator->emit, BC_CMPLT, 0, 0);
		}
		break;
		default:
		{
			c_error("%i\n", node->type);
		}
		break;
	}
	node_free(node);
}

void enter_literal(Generator *generator, Node *node) {
	emit(generator, "\tpush %s\n", (node->token->data));
	emit_opcode(generator->emit, BC_PUSH, atoi(node->token->data), 0);
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	emit(generator, "\tload %s\n", node->token->data);
	int i = emit_add_to_constant_pool(generator->emit, node->token->data);
	emit_opcode(generator->emit, BC_LOAD, i, 0);
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {
	if(node->args) {
		visitor(generator, node->args);
	}
	emit(generator, "\tcall %s\n", node->token->data);
	int i = emit_add_to_constant_pool(generator->emit, node->token->data);
	emit_opcode(generator->emit, BC_CALL, i, 0);
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
	unsigned line = emit_get_current_line(generator->emit);
	OP *jmp = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		emit_opcode(generator->emit, BC_PUSH, 0, 0);
		emit(generator, "\tpush %i\n", 0);
		jmp = emit_opcode(generator->emit, BC_JMPIFEQ, 0, 0);
		emit(generator, "\tjmpifeq ???\n");
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	emit_opcode(generator->emit, BC_GOTO, line, 0);
	emit(generator, "\tgoto %i\n", line);
	unsigned finish_line = emit_get_current_line(generator->emit);
	if(jmp) {
		jmp->left = finish_line;
	}
	node_free(node);
}

void enter_param(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_back(list)); // begin with end
		if(entry->type == AST_IDENT) {
			emit(generator, "\tstore %s\n", entry->token->data);
			int i = emit_add_to_constant_pool(generator->emit, entry->token->data);
			emit_opcode(generator->emit, BC_STORE, i, 0);
		}
		node_free(entry);
	}
	node_free(node);
}

void enter_arg(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	int count = 0;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
		count++;
	}
	emit_opcode(generator->emit, BC_PUSH, count, 0);
	node_free(node);
}

void enter_return(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	emit(generator, "\tret\n");
	emit_opcode(generator->emit, BC_RET, 0, 0);
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
	emit_opcode(generator->emit, BC_STRCONCAT, 0, 0);
	node_free(node);
}

void enter_string(Generator *generator, Node *node) {
	emit(generator, "\tpush_str %s\n", node->token->data);
	int i = emit_add_to_constant_pool(generator->emit, node->token->data);
	emit_opcode(generator->emit, BC_PUSHSTR, i, 0);
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
		case AST_GT:
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
	generator.file = fopen("data/out.code", "wb");
	generator.emit = new_emit();
	visitor(&generator, node);
	emit_build(generator.emit, "data/out.bin");
	free_emit(generator.emit);
	fclose(generator.file);
}
