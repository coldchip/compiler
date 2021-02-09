#include "codegen.h"

void emit(Generator *generator, const char *format, ...) {
	va_list args;
    va_start(args, format);
    vfprintf(generator->file, format, args);
    va_end(args);
}

void gen_store(Generator *generator, Node *node) {
	if(node->type == AST_IDENT_MEMBER) {
		if(node->index) {
			visitor(generator, node->index);
		}
		emit(generator, "\tarr_store %s\n", node->token->data);
		int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
		emit_opcode_1(generator->emit, BC_ARR_STORE, i);
	} else {
		emit(generator, "\tstore %s\n", node->token->data);
		int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
		emit_opcode_1(generator->emit, BC_STORE, i);
	}
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
	if(node->data_type & DATA_ARRAY_MASK) {
		// type a[x];
		node->data_type &= ~(DATA_ARRAY_MASK); // remove array bitmask
		if(node->body) {
			// char[] x = "abc";
			visitor(generator, node->body);
			emit(generator, "\tstore %s\n", node->token->data);
			int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
			emit_opcode_1(generator->emit, BC_STORE, i);
		} else if(node->size) {
			visitor(generator, node->size);
			emit(generator, "\tnewarray @type[%i]\n", node->data_type);
			emit_opcode_1(generator->emit, BC_NEWARRAY, node->data_type);
			emit(generator, "\tstore %s\n", node->token->data);
			int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
			emit_opcode_1(generator->emit, BC_STORE, i);
		} else {
			c_error("unable to emit array");
		}
	} else {
		// type a; or type a = xxx;
		if(node->body) {
			visitor(generator, node->body);
			emit(generator, "\tstore %s\n", node->token->data);
			int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
			emit_opcode_1(generator->emit, BC_STORE, i);
		}
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
			emit_opcode_0(generator->emit, BC_ADD);
		}
		break;
		case AST_SUB:
		{
			emit(generator, "\tsub\n");
			emit_opcode_0(generator->emit, BC_SUB);
		}
		break;
		case AST_MUL:
		{
			emit(generator, "\tmul\n");
			emit_opcode_0(generator->emit, BC_MUL);
		}
		break;
		case AST_DIV:
		{
			emit(generator, "\tdiv\n");
			emit_opcode_0(generator->emit, BC_DIV);
		}
		break;
		case AST_EQUAL:
		{
			emit(generator, "\tcmpeq\n");
			emit_opcode_0(generator->emit, BC_CMPEQ);
		}
		break;
		case AST_NOTEQUAL:
		{
			emit(generator, "\tcmpneq\n");
			emit_opcode_0(generator->emit, BC_CMPNEQ);
		}
		break;
		case AST_GT:
		{
			emit(generator, "\tcmpgt\n");
			emit_opcode_0(generator->emit, BC_CMPGT);
		}
		break;
		case AST_LT:
		{
			emit(generator, "\tcmplt\n");
			emit_opcode_0(generator->emit, BC_CMPLT);
		}
		break;
		case AST_SHL:
		{
			emit(generator, "\tshl\n");
			emit_opcode_0(generator->emit, BC_SHL);
		}
		break;
		case AST_SHR:
		{
			emit(generator, "\tshr\n");
			emit_opcode_0(generator->emit, BC_SHR);
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
	emit(generator, "\tpush_i %s\n", (node->token->data));
	emit_opcode_1(generator->emit, BC_PUSH_I, atoi(node->token->data));
	node_free(node);
}

void enter_char_literal(Generator *generator, Node *node) {
	emit(generator, "\tpush_i %i\n", (int)*(node->token->data));
	emit_opcode_1(generator->emit, BC_PUSH_I, (int)*(node->token->data));
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	emit(generator, "\tload %s\n", node->token->data);
	int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
	emit_opcode_1(generator->emit, BC_LOAD, i);
	node_free(node);
}

void enter_ident_member(Generator *generator, Node *node) {
	if(node->index) {
		visitor(generator, node->index);
	}
	emit(generator, "\tarr_load %s\n", node->token->data);
	int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
	emit_opcode_1(generator->emit, BC_ARR_LOAD, i);
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {
	if(node->args) {
		visitor(generator, node->args);
	}
	emit(generator, "\tcall %s\n", node->token->data);
	int i = 0;
	if(strstr(node->token->data, "__callinternal__") != NULL) {
		i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_STRING); // no obfuscation
	} else {
		i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_FNAME); // obfuscated
	}
	emit_opcode_1(generator->emit, BC_CALL, i);
	node_free(node);
}

void enter_if(Generator *generator, Node *node) {
	OP *jmp_exit = NULL;
	OP *jmp_exit_no_else = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		emit_opcode_1(generator->emit, BC_PUSH_I, 0);
		emit(generator, "\tpush_i %i\n", 0);
		jmp_exit = emit_opcode_1(generator->emit, BC_JMPIFEQ, 0);
		emit(generator, "\tjmpifeq ???\n");
	}
	if(node->body) {
		visitor(generator, node->body);
		jmp_exit_no_else = emit_opcode_1(generator->emit, BC_GOTO, 0);
		emit(generator, "\tgoto ???\n");
	}
	if(node->alternate) {
		jmp_exit->left = emit_get_current_line(generator->emit);
		visitor(generator, node->alternate);
	} else {
		jmp_exit->left = emit_get_current_line(generator->emit);
	}
	if(jmp_exit_no_else) {
		jmp_exit_no_else->left = emit_get_current_line(generator->emit);
	}
	node_free(node);
}

void enter_while(Generator *generator, Node *node) {
	unsigned line = emit_get_current_line(generator->emit);
	OP *jmp = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		emit_opcode_1(generator->emit, BC_PUSH_I, 0);
		emit(generator, "\tpush_i %i\n", 0);
		jmp = emit_opcode_1(generator->emit, BC_JMPIFEQ, 0);
		emit(generator, "\tjmpifeq ???\n");
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	emit_opcode_1(generator->emit, BC_GOTO, line);
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
			int i = emit_add_to_constant_pool(generator->emit, entry->token->data, CT_VARIABLE);
			emit_opcode_1(generator->emit, BC_STORE, i);
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
	emit(generator, "\tpush_i %i #args count\n", count);
	emit_opcode_1(generator->emit, BC_PUSH_I, count);
	node_free(node);
}

void enter_return(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	emit(generator, "\tret\n");
	emit_opcode_0(generator->emit, BC_RET);
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
	emit_opcode_0(generator->emit, BC_STRCONCAT);
	node_free(node);
}

void enter_string_literal(Generator *generator, Node *node) {
	emit(generator, "\tpush_s %s\n", node->token->data);
	int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_STRING);
	emit_opcode_1(generator->emit, BC_PUSH_S, i);
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
		case AST_NOTEQUAL:
		case AST_SHL:
		case AST_SHR:
		{
			enter_binexpr(generator, node);
		}
		break;
		case AST_LITERAL:
		{
			enter_literal(generator, node);
		}
		break;
		case AST_CHAR_LITERAL:
		{
			enter_char_literal(generator, node);
		}
		break;
		case AST_IDENT:
		{
			enter_ident(generator, node);
		}
		break;
		case AST_IDENT_MEMBER:
		{
			enter_ident_member(generator, node);
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
		case AST_STRING_LITERAL:
		{
			enter_string_literal(generator, node);
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
	//emit_build(generator.emit, "data/out.chip");
	emit_build2(generator.emit, "data/out.chip");
	emit_asm(generator.emit, "data/out.asm");
	free_emit(generator.emit);
	fclose(generator.file);
}
