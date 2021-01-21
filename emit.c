#include "emit.h"

Emit *new_emit() {
	Emit *emit = malloc(sizeof(Emit));
	if(!emit) {
		return NULL;
	}
	emit->constant_pool_index = 0;
	list_clear(&emit->constant_pool);
	list_clear(&emit->code);
	return emit;
}

int emit_add_to_constant_pool(Emit *emit, char *string) {
	for(ListNode *i = list_begin(&emit->constant_pool); i != list_end(&emit->constant_pool); i = list_next(i)) {
		ConstantPoolRow *row = (ConstantPoolRow*)i;
		if(strcmp(row->data, string) == 0) {
			return row->index;
		}
	}
	ConstantPoolRow *row = malloc(sizeof(ConstantPoolRow));
	row->index = emit->constant_pool_index++;
	row->data = strmalloc(string);
	list_insert(list_end(&emit->constant_pool), row);
	return row->index;
}

void emit_opcode(Emit *emit, ByteCode op, int left, int right) {
	OP *row = malloc(sizeof(OP));
	row->op = op;
	row->left = left;
	row->right = right;
	list_insert(list_end(&emit->code), row);
}

void emit_build(Emit *emit, char *file) {
	FILE *fp = fopen(file, "wb");
	fwrite(&emit->constant_pool_index, sizeof(unsigned), 1, fp);
	for(ListNode *i = list_begin(&emit->constant_pool); i != list_end(&emit->constant_pool); i = list_next(i)) {
		ConstantPoolRow *row = (ConstantPoolRow*)i;
		unsigned str_len = strlen(row->data);
		char *data = row->data;
		fwrite(&str_len, sizeof(unsigned), 1, fp);
		fwrite(data, sizeof(char), str_len, fp);
	}
	unsigned code_count = list_size(&emit->code);
	fwrite(&code_count, sizeof(unsigned), 1, fp);
	for(ListNode *i = list_begin(&emit->code); i != list_end(&emit->code); i = list_next(i)) {
		OP *row = (OP*)i;
		uint8_t op = (uint8_t)row->op;
		int left = (int)row->left;
		int right = (int)row->right;
		fwrite(&op, sizeof(uint8_t), 1, fp);
		//fprintf(fp, "L");
		fwrite(&left, sizeof(int), 1, fp);
		//fprintf(fp, "R");
		fwrite(&right, sizeof(int), 1, fp);
		//fprintf(fp, "E");
	}
	fclose(fp);
}

void free_emit(Emit *emit) {
	free(emit);
}