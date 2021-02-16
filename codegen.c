#include "codegen.h"

void emit(Generator *generator, const char *format, ...) {
	va_list args;
    va_start(args, format);
    vfprintf(generator->file, format, args);
    va_end(args);
}

void gen_store(Generator *generator, Node *node) {
	if(node->type == AST_DEREF) {
		Node *body = node->body;
		emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_ADDR, BC_MOV, REG_1, body->offset, "store");
		emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOVIND2, REG_1, REG_0, "store");
	} else {
		emit_opcode(generator->emit, BM_L | BM_L_ADDR | BM_R | BM_R_REG, BC_MOV, node->offset, REG_0, "store");
	}
	node_free(node);
}

void gen_addr(Generator *generator, Node *node) {
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_ADDR, BC_LEA, REG_0, node->offset, "genaddr");
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_ADD, REG_0, FP, "add base to ptr");
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
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, FP, 0, "prologue");
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, FP, SP, "prologue");

	// stack reserve for local
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_MOV, REG_0, node->total_local_size, "local size");
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_ADD, SP, REG_0, "allocate local variables");
	if(node->args) {
		visitor(generator, node->args);
	}
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_begin(list));
		visitor(generator, entry);
	}

	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, SP, FP, "epilogue");
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_POP, FP, 0, "epilogue");
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
		emit_opcode(generator->emit, BM_L | BM_L_ADDR | BM_R | BM_R_REG, BC_MOV, node->offset, REG_0, "store variable");
	}
	
	node_free(node);
}

void enter_binexpr(Generator *generator, Node *node) {
	/* Enter deepest first */
	if(node->right) {
		visitor(generator, node->right);
	}
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, REG_0, 0, "push right");
	if(node->left) {
		visitor(generator, node->left);
	}
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, REG_0, 0, "push left");

	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_POP, REG_0, 0, "pop right");
	emit_opcode(generator->emit, BM_L | BM_L_REG, BC_POP, REG_1, 0, "pop left");
	switch(node->type) {
		case AST_LOGAND: 
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_CMP, REG_0, 1, "logical and cmp #1");
			emit_opcode(generator->emit, BM_L | BM_L_REG, BC_SETEEQ, REG_0, 0, "seteeq (expr)");

			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_CMP, REG_1, 1, "logical and cmp #2");
			emit_opcode(generator->emit, BM_L | BM_L_REG, BC_SETEEQ, REG_1, 0, "seteeq (expr)");

			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_AND, REG_0, REG_1, "AND logical and");

			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_CMP, REG_0, 1, "logand cmp AND'ed");
			emit_opcode(generator->emit, BM_L | BM_L_REG, BC_SETEEQ, REG_0, 0, "seteeq (expr)");
		}
		break;
		case AST_ADD:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_ADD, REG_0, REG_1, "add");
		}
		break;
		case AST_SUB:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_SUB, REG_0, REG_1, "subtract");
		}
		break;
		case AST_MUL:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MUL, REG_0, REG_1, "multiply");
		}
		break;
		case AST_DIV:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_DIV, REG_0, REG_1, "divide");
		}
		break;
		case AST_MOD:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOD, REG_0, REG_1, "mod");
		}
		break;
		case AST_EQUAL:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_CMP, REG_0, REG_1, "cmp (expr)");
			emit_opcode(generator->emit, BM_L | BM_L_REG, BC_SETEEQ, REG_0, 0, "seteeq (expr)");
		}
		break;
		case AST_NOTEQUAL:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_CMP, REG_0, REG_1, "cmp (expr)");
			emit_opcode(generator->emit, BM_L | BM_L_REG, BC_SETENEQ, REG_0, 0, "seteneq (expr)");
		}
		break;
		case AST_GT:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_CMP, REG_0, REG_1, "cmp (expr)");
			emit_opcode(generator->emit, BM_L | BM_L_REG, BC_SETEGT, REG_0, 0, "setegt (expr)");
		}
		break;
		case AST_LT:
		{
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_CMP, REG_0, REG_1, "cmp (expr)");
			emit_opcode(generator->emit, BM_L | BM_L_REG, BC_SETELT, REG_0, 0, "setelt (expr)");
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
	node_free(node);
}

void enter_literal(Generator *generator, Node *node) {
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_MOV, REG_0, atoi(node->token->data), "move literal to register 0");
	node_free(node);
}

void enter_char_literal(Generator *generator, Node *node) {
	
	// emit_opcode_1(generator->emit, BC_PUSH_I, (int)*(node->token->data));
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_ADDR, BC_MOV, REG_0, node->offset, "access variable");
	node_free(node);
}

void enter_ident_member(Generator *generator, Node *node) {
	if(node->index) {
		visitor(generator, node->index);
	}
	
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, REG_9, SP, "preserve arguments pointer #1 (fixed)");
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, REG_10, SP, "preserve arguments pointer #2 (unfixed)");
	if(node->args) {
		visitor(generator, node->args);
	}
	
	int i = emit_add_to_constant_pool(generator->emit, node->token->data, CT_STRING);
	emit_opcode(generator->emit, BM_L | BM_L_ADDR, BC_CALL, i, 0, "call");
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, SP, REG_9, "restore sp from args");
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, REG_0, REG_11, "move return [r11] to local");
	node_free(node);
}

void enter_if(Generator *generator, Node *node) {	
	unsigned line = emit_get_current_line(generator->emit);
	OP *jmp = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_CMP, REG_0, 1, "cmp (while)");
		jmp = emit_opcode(generator->emit, BM_L | BM_L_ADDR, BC_JNE, line, 0, "jmp finish(while)");
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	if(jmp) {
		jmp->left = emit_get_current_line(generator->emit);
	}
	node_free(node);
}

void enter_while(Generator *generator, Node *node) {
	unsigned line = emit_get_current_line(generator->emit);
	OP *jmp = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_CMP, REG_0, 1, "cmp (while)");
		jmp = emit_opcode(generator->emit, BM_L | BM_L_ADDR, BC_JNE, line, 0, "jmp finish(while)");
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	emit_opcode(generator->emit, BM_L | BM_L_ADDR, BC_JMP, line, 0, "jmp (while)");
	if(jmp) {
		jmp->left = emit_get_current_line(generator->emit);
	}
	node_free(node);
}

void enter_param(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_back(list)); // begin with end
		if(entry->type == AST_IDENT) {
			emit_opcode(generator->emit, BM_L | BM_L_ADDR | BM_R | BM_R_REG, BC_MOVIND, entry->offset, REG_10, "mov arguments to local stack");
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_MOV, REG_0, entry->size, "move arg size");
			emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_ADD, REG_10, REG_0, "increment the arg size");
		}
		node_free(entry);
	}
	node_free(node);
}

void enter_arg(Generator *generator, Node *node) {
	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_back(list));
		visitor(generator, entry);
		emit_opcode(generator->emit, BM_L | BM_L_REG, BC_PUSH, REG_0, 0, "push arg to stack");
	}
	
	node_free(node);
}

void enter_return(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOV, REG_11, REG_0, "move data to return reg [r11]");
	node_free(node);
}

void enter_string_concat(Generator *generator, Node *node) {
	if(node->right) {
		visitor(generator, node->right);
	}
	if(node->left) {
		visitor(generator, node->left);
	}
	
	node_free(node);
}

void enter_string_literal(Generator *generator, Node *node) {
	
	node_free(node);
}

void enter_derefrence(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_REG, BC_MOVIND, REG_0, REG_0, "deref");

	node_free(node);
}

void enter_refrence(Generator *generator, Node *node) {
	if(node->body) {
		gen_addr(generator, node->body);
	}
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
		case AST_LOGAND:
		case AST_ADD:
		case AST_SUB:
		case AST_MUL:
		case AST_DIV:
		case AST_MOD:
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
	emit_asm(generator.emit, "data/out.S");
	free_emit(generator.emit);
	fclose(generator.file);
}
