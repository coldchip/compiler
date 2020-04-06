#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

char *read_file_into_buffer(char *file) {
	FILE *infp = fopen(file, "rb");
    if (!infp) {
    	printf("Cannot open %s\n", file);
    	exit(0);
    }
    fseek(infp, 0, SEEK_END);
	long fsize = ftell(infp);
	char *p = malloc(fsize + 1);
	fseek(infp, 0, SEEK_SET);

	fread((char*)p, 1, fsize, infp);
	fclose(infp);
	*(p + fsize) = '\0';


	for(char *check = p; check < p + fsize; check++) {
		if(*check == '\0') {
			printf("Cannot compiler because file %s contains NULL character(s)\n", file);
    		exit(0);
		}
	}
	return p;
}

int main(int argc, char const *argv[]) {
	char *file = (char*)argv[1];
	if(file != NULL) {
		printf("Attempting to compile %s\n", file);
		char *buf = read_file_into_buffer(file);
		printf("Preprocessing %s\n", file);
		char *expanded = macro(buf);
		printf("Lexing %s\n", file);
		Token *token = lex(expanded);
		printf("Parsing %s\n", file);
		ASTNode *node = parse(token);
		printf("Generating Byte Code %s\n", file);
		GenState *gs = generate(node);


		FILE *out = fopen("a.out", "wb");
		if(!out) {
			printf("cannot open file for writing\n");
			exit(1);
		}
		ChipBinary cb;
		memset(&cb, 0, sizeof(cb));
		strcpy(cb.magic, "CBIN");
		cb.version = 1;

		fwrite((char*)&cb, 1, sizeof(cb), out);

		InstructionEntry *cs = gs->ir->start;

		while(cs != NULL) {
			int op = (int)cs->op;
			int left = (int)cs->left;
			int right = (int)cs->right;
			printf("%i %i %i\n", op, left, right);
			fwrite((char*)&op, 1, sizeof(int), out);
			fwrite((char*)&left, 1, sizeof(int), out);
			fwrite((char*)&right, 1, sizeof(int), out);
			cs = cs->next;
		}

		fclose(out);

		free(buf);
	} else {
		printf("No input file(s) specified\n");
	}
	
}