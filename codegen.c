#include "codegen.h"

int counter() {
	static int i = 0;
	return i++;
}

void gen_store(Generator *generator, Node *node) {
	if(node->type == AST_DEREF) {
		Node *body = node->body;
		visitor(generator, body);
		fprintf(generator->file, "\tstore_ptr\n");
	} else {
		fprintf(generator->file, "\tstore %s\n", node->token->data);
	}
	node_free(node);
}

void gen_addr(Generator *generator, Node *node) {
	fprintf(generator->file, "\tload_addr %s\n", node->token->data);
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
	fprintf(generator->file, "%s:\n", node->token->data);
	// prologue
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
		fprintf(generator->file, "\tstore %s\n", node->token->data);
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
		case AST_LOGAND: 
		{
	
		}
		break;
		case AST_ADD:
		{
			fprintf(generator->file, "\tadd\n");
		}
		break;
		case AST_SUB:
		{
			fprintf(generator->file, "\tsub\n");
		}
		break;
		case AST_MUL:
		{
			fprintf(generator->file, "\tmul\n");
		}
		break;
		case AST_DIV:
		{
			fprintf(generator->file, "\tdiv\n");
		}
		break;
		case AST_MOD:
		{
			fprintf(generator->file, "\tmod\n");
		}
		break;
		case AST_EQUAL:
		{
			fprintf(generator->file, "\tequal\n");
		}
		break;
		case AST_NOTEQUAL:
		{
			fprintf(generator->file, "\taabcmp\n");
		}
		break;
		case AST_GT:
		{
			fprintf(generator->file, "\tCMP(r0, r1)\n");
			fprintf(generator->file, "\tSETE(r0)\n");
		}
		break;
		case AST_LT:
		{
			fprintf(generator->file, "\tCMP(r0, r1)\n");
			fprintf(generator->file, "\tSETE(r0)\n");
		}
		break;
		case AST_SHL:
		{
			
		}
		break;
		case AST_SHR:
		{
			
		}
		break;
		case AST_AND:
		{
			
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
	fprintf(generator->file, "\tpush_int %i\n", atoi(node->token->data));
	node_free(node);
}

void enter_char_literal(Generator *generator, Node *node) {
	
	node_free(node);
}

void enter_ident(Generator *generator, Node *node) {
	fprintf(generator->file, "\tload %s\n", node->token->data);
	node_free(node);
}

void enter_ident_member(Generator *generator, Node *node) {
	if(node->index) {
		visitor(generator, node->index);
	}
	
	node_free(node);
}

void enter_call(Generator *generator, Node *node) {	
	if(strcmp(node->token->data, "__asm__") == 0) {
		Node *ins = node->args;
		Node *entry = (Node*)list_remove(list_back(&ins->bodylist));
		fprintf(generator->file, "%s\n", entry->token->data);
	} else {
		if(node->args) {
			visitor(generator, node->args);
		}
		fprintf(generator->file, "\tcall %s\n", node->token->data);
	}
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
	int r = counter();
	emit_label(generator->emit, "if.%i", r);
	if(jmp) {
		jmp->left = emit_get_label_addr(generator->emit, "if.%i", r);
	}
	node_free(node);
}

void enter_while(Generator *generator, Node *node) {
	int c_c = counter();
	int d_c = counter();
	fprintf(generator->file, "w_%i:\n", c_c);
	int line = emit_label(generator->emit, "while.%i", c_c);
	OP *jmp = NULL;
	if(node->condition) {
		visitor(generator, node->condition);
		fprintf(generator->file, "\tcmp 1\n");
		emit_opcode(generator->emit, BM_L | BM_L_REG | BM_R | BM_R_VAL, BC_CMP, REG_0, 1, "cmp (while)");
		fprintf(generator->file, "\tje w_%i\n", d_c);
		jmp = emit_opcode(generator->emit, BM_L | BM_L_ADDR, BC_JNE, line, 0, "jmp finish(while)");
	}
	if(node->body) {
		visitor(generator, node->body);
	}
	fprintf(generator->file, "\tgoto w_%i\n", c_c);	
	emit_opcode(generator->emit, BM_L | BM_L_ADDR, BC_JMP, line, 0, "jmp (while)");
	fprintf(generator->file, "w_%i:\n", d_c);	
	if(jmp) {
		jmp->left = emit_label(generator->emit, "while.%i", counter());
	}
	node_free(node);
}

void enter_param(Generator *generator, Node *node) {
	int16_t offset = 0;

	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_back(list)); // begin with end
		if(entry->type == AST_IDENT) {
			offset += 4;
		}
		node_free(entry);
	}
	node_free(node);
}

void enter_arg(Generator *generator, Node *node) {
	int count = 0;

	List *list = &node->bodylist;
	while(!list_empty(list)) {
		Node *entry = (Node*)list_remove(list_back(list));
		visitor(generator, entry);
		count++;
	}
	
	node_free(node);
}

void enter_return(Generator *generator, Node *node) {
	if(node->body) {
		visitor(generator, node->body);
	}
	
	fprintf(generator->file, "\tret\n");
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
	fprintf(generator->file, "\tderef\n");
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
	generator.file = fopen("data/code.S", "wb");
	generator.emit = new_emit();
	visitor(&generator, node);
	free_emit(generator.emit);
	fclose(generator.file);
}
