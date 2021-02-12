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
		
		int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
		// emit_opcode_1(generator->emit, BC_ARR_STORE, i);
	} else {
		
		int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_VARIABLE);
		// emit_opcode_1(generator->emit, BC_STORE, i);
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
	emit_select_function(generator->emit, node->token->data);
	// prologue
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, FP, 0);
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, FP, SP);

	// stack reserve for local
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_MOV, REG_0, node->total_local_size);
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_ADD, SP, REG_0);
	if(node->args) {
		visitor(generator, node->args);
	}
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}

	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, SP, FP);
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_POP, FP, 0);
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
		emit_opcode(generator->emit, BM_L | BM_L_ADDR | BM_R | BM_R_REG, BC_MOV, node->offset, REG_0);
	}
	
	node_free(node);
}

void enter_binexpr(Generator *generator, Node *node) {
	/* Enter deepest first */
	if(node->right) {
		visitor(generator, node->right);
	}
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, REG_0, 0);
	if(node->left) {
		visitor(generator, node->left);
	}
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, REG_0, 0);

	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_POP, REG_0, 0);
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_POP, REG_1, 0);
	switch(node->type) {
		case AST_ADD:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_ADD, REG_0, REG_1);
			// emit_opcode_0(generator->emit, BC_ADD);
		}
		break;
		case AST_SUB:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_SUB, REG_0, REG_1);
			// emit_opcode_0(generator->emit, BC_SUB);
		}
		break;
		case AST_MUL:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MUL, REG_0, REG_1);
			// emit_opcode_0(generator->emit, BC_MUL);
		}
		break;
		case AST_DIV:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_DIV, REG_0, REG_1);
			// emit_opcode_0(generator->emit, BC_DIV);
		}
		break;
		case AST_EQUAL:
		{
			
			// emit_opcode_0(generator->emit, BC_CMPEQ);
		}
		break;
		case AST_NOTEQUAL:
		{
			
			// emit_opcode_0(generator->emit, BC_CMPNEQ);
		}
		break;
		case AST_GT:
		{
			
			// emit_opcode_0(generator->emit, BC_CMPGT);
		}
		break;
		case AST_LT:
		{
			
			// emit_opcode_0(generator->emit, BC_CMPLT);
		}
		break;
		case AST_SHL:
		{
			
			// emit_opcode_0(generator->emit, BC_SHL);
		}
		break;
		case AST_SHR:
		{
			
			// emit_opcode_0(generator->emit, BC_SHR);
		}
		break;
		case AST_AND:
		{
			
			// emit_opcode_0(generator->emit, BC_AND);
		}
		break;
		default:
		{
			c_error("%i\n", node->type);
		}
		break;
	}
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, REG_0, 0);
	node_free(node);
}

void enter_literal(Generator *generator, Node *node) {
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_MOV, REG_0, atoi(node->token->data));
	node_free(node);
}

void enter_char_literal(Generator *generator, Node *node) {
	
	// emit_opcode_1(generator->emit, BC_PUSH_I, (int)*(node->token->data));
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_ADDR, BC_MOV, REG_0, node->offset);
	node_free(node);
}

void enter_ident_member(Generator *generator, Node *node) {
	if(node->index) {
		visitor(generator, node->index);
	}
	
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {
	if(node->args) {
		visitor(generator, node->args);
	}
	
	int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_STRING);
	emit_opcode(generator->emit, BM_L | BM_L_ADDR, BC_CALL, i, 0);
	// emit_opcode_1(generator->emit, BC_CALL, i);
	node_free(node);
}

void enter_if(Generator *generator, Node *node) {
	/*
	OP *jmp_exit = NULL;
	OP *jmp_exit_no_else = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		// emit_opcode_1(generator->emit, BC_PUSH_I, 0);
		
		jmp_exit = // emit_opcode_1(generator->emit, BC_JMPIFEQ, 0);
		
	}
	if(node->body) {
		visitor(generator, node->body);
		jmp_exit_no_else = // emit_opcode_1(generator->emit, BC_GOTO, 0);
		
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
	*/
	node_free(node);
}

void enter_while(Generator *generator, Node *node) {
	/*
	unsigned line = emit_get_current_line(generator->emit);
	OP *jmp = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		// emit_opcode_1(generator->emit, BC_PUSH_I, 0);
		
		jmp = // emit_opcode_1(generator->emit, BC_JMPIFEQ, 0);
		
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	// emit_opcode_1(generator->emit, BC_GOTO, line);
	
	unsigned finish_line = emit_get_current_line(generator->emit);
	if(jmp) {
		jmp->left = finish_line;
	}
	*/
	node_free(node);
}

void enter_param(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_back(list)); // begin with end
		if(entry->type == AST_IDENT) {
			
			int i = emit_add_to_constant_pool(generator->emit, entry->token->data, CT_VARIABLE);
			// emit_opcode_1(generator->emit, BC_STORE, i);
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
	
	// emit_opcode_1(generator->emit, BC_PUSH_I, count);
	node_free(node);
}

void enter_return(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	
	// emit_opcode_0(generator->emit, BC_RET);
	node_free(node);
}

void enter_string_concat(Generator *generator, Node *node) {
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		visitor(generator, node->left);
	}
	
	// emit_opcode_0(generator->emit, BC_STRCONCAT);
	node_free(node);
}

void enter_string_literal(Generator *generator, Node *node) {
	
	int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_STRING);
	// emit_opcode_1(generator->emit, BC_PUSH_S, i);
	node_free(node);
}

void enter_derefrence(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	// emit_opcode_0(generator->emit, BC_DEREF);
	node_free(node);
}

void enter_refrence(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	// emit_opcode_0(generator->emit, BC_REF);
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
		case AST_AND:
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
		case AST_DEREF:
		{
			enter_derefrence(generator, node);
		}
		break;
		case AST_REF:
		{
			enter_refrence(generator, node);
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
