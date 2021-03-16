#include "emit.h"

char *bc_char[] = {
	"nop", // do nothing
	"newarray",
	"arr_store",
	"arr_load",
	"add",
	"sub",
	"mul",
	"div",
	"mod",
	"shl",
	"shr",
	"and",
	"strconcat",
	"store",
	"load",
	"push",
	"pop",
	"call",
	"ret",
	"cmpeq",
	"cmpneq",
	"cmpgt",
	"cmplt",
	"jne",
	"je",
	"jmp",
	"deref",
	"ref",
	"mov",
	"movind",
	"movind2",
	"cmp",
	"setegt",
	"setelt",
	"seteeq",
	"seteneq",
	"lea",
	"syscall",
	"halt"
};

Emit *new_emit() {
	Emit *emit = malloc(sizeof(Emit));
	if(!emit) {
		return NULL;
	}
	emit->constant_pool_index = 0;
	list_clear(&emit->constant_pool);
	list_clear(&emit->code);
	list_clear(&emit->label);
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

int emit_label(Emit *emit, char *format, ...) {
	va_list args;
	va_start(args, format);

	char buf[1000];

	vsprintf(buf, format, args);

	va_end(args);

	Label *label = malloc(sizeof(Label));
	label->name = strmalloc(buf);
	label->addr = emit_get_current_line(emit);
	list_insert(list_end(&emit->label), label);

	return label->addr;
}

int emit_get_label_addr(Emit *emit, char *format, ...) {
	va_list args;
	va_start(args, format);

	char buf[1000];

	vsprintf(buf, format, args);

	va_end(args);

	for(ListNode *l = list_begin(&emit->label); l != list_end(&emit->label); l = list_next(l)) {
		Label *label = (Label*)l;
		if(strcmp(buf, label->name) == 0) {
			return label->addr;
		}
	}
	c_error("label %s not found", buf);
	return -1;
}

char *emit_get_label_name(Emit *emit, int addr) {
	for(ListNode *l = list_begin(&emit->label); l != list_end(&emit->label); l = list_next(l)) {
		Label *label = (Label*)l;
		if(addr == label->addr) {
			return label->name;
		}
	}
	return NULL;
}

unsigned emit_get_current_line(Emit *emit) {
	return list_size(&emit->code);
}

OP *emit_opcode(Emit *emit, ByteMode mode, ByteCode op, int left, int right, char *comments) {
	OP *row = malloc(sizeof(OP));
	row->op = op;
	row->left = left;
	row->right = right;
	row->mode = 0;
	row->mode |= mode;
	row->comments = NULL;
	if(comments) {
		row->comments = strmalloc(comments);
	}
	list_insert(list_end(&emit->code), row);
	return row;
}


void emit_asm(Emit *emit, char *file) {
	FILE *fp = fopen(file, "wb");

	
	int counter = 0;
	for(ListNode *c = list_begin(&emit->code); c != list_end(&emit->code); c = list_next(c)) {
		OP *row = (OP*)c;
		uint8_t op = (uint8_t)row->op;
		int left = (int)row->left;
		int right = (int)row->right;
		char *comments = row->comments;

		uint8_t  reg_l = ((left >> 0) & 0xFF);
		int16_t  off_l = ((left >> 8) & 0xFFFF);

		uint8_t  reg_r = ((right >> 0) & 0xFF);
		int16_t  off_r = ((right >> 8) & 0xFFFF);

		for(ListNode *l = list_begin(&emit->label); l != list_end(&emit->label); l = list_next(l)) {
			Label *label = (Label*)l;
			if(label->addr == counter) {
				fprintf(fp, "%s:\n", label->name);
			}
		}

		fprintf(fp, "\t%s ", bc_char[op]);
		if(row->mode & BM_L) {
			if(row->mode & BM_L_IND) {
				fprintf(fp, "PTR ");
			}
			if(row->mode & BM_L_REG) {
				if(off_l != 0) {
					fprintf(fp, "%i(", off_l);
				}
				if(reg_l == SP) {
					fprintf(fp, "%%sp");
				} else if(reg_l == FP) {
					fprintf(fp, "%%fp");
				} else if(reg_l == IP) {
					fprintf(fp, "%%ip");
				} else {
					fprintf(fp, "%%r%i", reg_l);
				}
				if(off_l != 0) {
					fprintf(fp, ")");
				}
			} else if(row->mode & BM_L_ADDR) {
				if(op == BC_CALL || op == BC_JE || op == BC_JNE || op == BC_JMP) {
					char *l_name = emit_get_label_name(emit, left);
					if(l_name) {
						fprintf(fp, "%s", l_name);
					} else {
						fprintf(fp, "@%i", left);
					}
				} else {
					fprintf(fp, "@%i", left);
				}
			} else if(row->mode & BM_L_VAL) {
				fprintf(fp, "$%i", left);
			} else {
				c_error("unknown opcode mode given");
			}
		}
		if(row->mode & BM_R) {
			fprintf(fp, ", ");
			if(row->mode & BM_R_IND) {
				fprintf(fp, "PTR ");
			}
			if(row->mode & BM_R_REG) {
				if(off_r != 0) {
					fprintf(fp, "%i(", off_r);
				}
				if(reg_r == SP) {
					fprintf(fp, "%%sp");
				} else if(reg_r == FP) {
					fprintf(fp, "%%fp");
				} else if(reg_r == IP) {
					fprintf(fp, "%%ip");
				} else {
					fprintf(fp, "%%r%i", reg_r);
				}
				if(off_r != 0) {
					fprintf(fp, ")");
				}
			} else if(row->mode & BM_R_ADDR) {
				fprintf(fp, "@%i", right);
			} else if(row->mode & BM_R_VAL) {
				fprintf(fp, "$%i", right);
			} else {
				c_error("unknown opcode mode given");
			}
		}
		if(comments) {
			fprintf(fp, "          # %s", comments);
		}
		fprintf(fp, "\n");
		counter++;
	}
	
	fclose(fp);
}



void emit_build2(Emit *emit, char *file) {
	FILE *fp = fopen(file, "wb");

	Header header;
	header.magic   = 1178944383;
	header.version = 5;
	header.time    = time(NULL);

	fwrite(&header, sizeof(Header), 1, fp);

	unsigned entry_point = emit_get_label_addr(emit, "main");
	fwrite(&entry_point, sizeof(unsigned), 1, fp);

	
	unsigned code_length = list_size(&emit->code);
	fwrite(&code_length, sizeof(unsigned), 1, fp);

	for(ListNode *c = list_begin(&emit->code); c != list_end(&emit->code); c = list_next(c)) {
		OP *row = (OP*)c;
		uint16_t op = (uint16_t)row->op;
		int mode    = (int)row->mode;
		int left    = (int)row->left;
		int right   = (int)row->right;

		if(fwrite(&mode, sizeof(uint16_t), 1, fp) != 1) {
			c_error("unable to emit bytecode to file");
		}

		if(fwrite(&op, sizeof(uint8_t), 1, fp) != 1) {
			c_error("unable to emit bytecode to file");
		}

		if(mode & BM_L) {
			if(fwrite(&left, sizeof(int), 1, fp) != 1) {
				c_error("unable to emit bytecode to file");
			}
		}

		if(mode & BM_R) {
			if(fwrite(&right, sizeof(int), 1, fp) != 1) {
				c_error("unable to emit bytecode to file");
			}
		}
	}
	

	fwrite(&emit->constant_pool_index, sizeof(unsigned), 1, fp);

	int count = 0;

	for(ListNode *i = list_begin(&emit->constant_pool); i != list_end(&emit->constant_pool); i = list_next(i)) {
		ConstantPoolRow *row = (ConstantPoolRow*)i;

		if(row->type == CT_STRING || OBFUSCATION == 0) {
			unsigned str_len = strlen(row->data);
			char *data = row->data;
			fwrite(&str_len, sizeof(unsigned), 1, fp);
			fwrite(data, sizeof(char), str_len, fp);
		} else if(row->type == CT_VARIABLE) {
			char format[] = "v%i";
			int size = snprintf(NULL, 0, format, count);
			char data[size + 1];
			sprintf(data, format, count);
			unsigned str_len = strlen(data);
			fwrite(&str_len, sizeof(unsigned), 1, fp);
			fwrite(data, sizeof(char), str_len, fp);
		} else if(row->type == CT_FNAME) {
			char format[] = "f%i";
			int size = snprintf(NULL, 0, format, count);
			char data[size + 1];
			sprintf(data, format, count);
			unsigned str_len = strlen(data);
			fwrite(&str_len, sizeof(unsigned), 1, fp);
			fwrite(data, sizeof(char), str_len, fp);
		} else {
			c_error("emit error, unable to emit constant_pool_row");
		}
		count++;
	}
	fclose(fp);
}



void free_emit(Emit *emit) {
	

	ListNode *c = list_begin(&emit->code);
	while(c != list_end(&emit->code)) {
		OP *row = (OP*)c;
		c = list_next(c);
		
		list_remove(&row->node);
		free(row);
	}

	ListNode *i = list_begin(&emit->constant_pool);
	while(i != list_end(&emit->constant_pool)) {
		ConstantPoolRow *row = (ConstantPoolRow*)i;
		i = list_next(i);

		list_remove(&row->node);
		free(row->data);
		free(row);
	}

	free(emit);
}