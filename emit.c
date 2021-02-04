#include "emit.h"

Emit *new_emit() {
	Emit *emit = malloc(sizeof(Emit));
	if(!emit) {
		return NULL;
	}
	emit->constant_pool_index = 0;
	list_clear(&emit->constant_pool);
	list_clear(&emit->functions);
	return emit;
}

int emit_add_to_constant_pool(Emit *emit, char *string, ConstantType type) {
	for(ListNode *i = list_begin(&emit->constant_pool); i != list_end(&emit->constant_pool); i = list_next(i)) {
		ConstantPoolRow *row = (ConstantPoolRow*)i;
		if(strcmp(row->data, string) == 0) {
			return row->index;
		}
	}
	ConstantPoolRow *row = malloc(sizeof(ConstantPoolRow));
	row->type = type;
	row->index = emit->constant_pool_index++;
	row->data = strmalloc(string);
	list_insert(list_end(&emit->constant_pool), row);
	return row->index;
}

void emit_select_function(Emit *emit, char *name) {
	Function *row = malloc(sizeof(Function));
	row->name = strmalloc(name);
	list_clear(&row->code);
	
	list_insert(list_end(&emit->functions), row);
	emit->current_function = row;
}

unsigned emit_get_current_line(Emit *emit) {
	return list_size(&emit->current_function->code);
}

OP *emit_opcode(Emit *emit, ByteCode op, int left, int right) {
	OP *row = malloc(sizeof(OP));
	row->op = op;
	row->left = left;
	row->right = right;

	list_insert(list_end(&emit->current_function->code), row);
	return row;
}

void emit_build(Emit *emit, char *file) {
	FILE *fp = fopen(file, "wb");

	Header header;
	header.magic   = 1178944383;
	header.version = 2;
	header.time    = time(NULL);

	fwrite(&header, sizeof(Header), 1, fp);

	unsigned f_count = list_size(&emit->functions);
	fwrite(&f_count, sizeof(unsigned), 1, fp);

	ListNode *f = list_begin(&emit->functions);
	while(f != list_end(&emit->functions)) {
		Function *function = (Function*)f;
		f = list_next(f);

		unsigned f_index = -1;
		if(strcmp(function->name, "main") == 0) {
			f_index = emit_add_to_constant_pool(emit, function->name, CT_STRING);
		} else {
			f_index = emit_add_to_constant_pool(emit, function->name, CT_FNAME);
		}
		unsigned f_length = list_size(&function->code);
		fwrite(&f_index, sizeof(unsigned), 1, fp);
		fwrite(&f_length, sizeof(unsigned), 1, fp);

		ListNode *c = list_begin(&function->code);
		while(c != list_end(&function->code)) {
			OP *row = (OP*)c;
			c = list_next(c);
			uint8_t op = (uint8_t)row->op;
			int left = (int)row->left;
			int right = (int)row->right;
			if(fwrite(&op, sizeof(uint8_t), 1, fp) != 1) {
				c_error("unable to emit bytecode to file");
			}
			if(fwrite(&left, sizeof(int), 1, fp) != 1) {
				c_error("unable to emit bytecode to file");
			}
			if(fwrite(&right, sizeof(int), 1, fp) != 1) {
				c_error("unable to emit bytecode to file");
			}
			list_remove(&row->node);
			free(row);
		}

		list_remove(&function->node);
		free(function->name);
		free(function);
	}

	fwrite(&emit->constant_pool_index, sizeof(unsigned), 1, fp);

	int count = 0;

	ListNode *i = list_begin(&emit->constant_pool);
	while(i != list_end(&emit->constant_pool)) {
		ConstantPoolRow *row = (ConstantPoolRow*)i;
		i = list_next(i);

		if(row->type == CT_STRING) {
			unsigned str_len = strlen(row->data);
			char *data = row->data;
			fwrite(&str_len, sizeof(unsigned), 1, fp);
			fwrite(data, sizeof(char), str_len, fp);
		} else if(row->type == CT_VARIABLE) {
			char format[] = "var_%i";
			int size = snprintf(NULL, 0, format, count);
			char data[size + 1];
			sprintf(data, format, count);
			unsigned str_len = strlen(data);
			fwrite(&str_len, sizeof(unsigned), 1, fp);
			fwrite(data, sizeof(char), str_len, fp);
		} else if(row->type == CT_FNAME) {
			char format[] = "func_%i";
			int size = snprintf(NULL, 0, format, count);
			char data[size + 1];
			sprintf(data, format, count);
			unsigned str_len = strlen(data);
			fwrite(&str_len, sizeof(unsigned), 1, fp);
			fwrite(data, sizeof(char), str_len, fp);
		} else {
			c_error("emit error, unable to emit constant_pool_row");
		}

		list_remove(&row->node);
		free(row->data);
		free(row);
		count++;
	}
	fclose(fp);
}

void free_emit(Emit *emit) {
	free(emit);
}