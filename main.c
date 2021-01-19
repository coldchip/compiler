#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chipcode.h"
#include "lex.h"
#include "parse.h"
#include "codegen.h"

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

	if(fread((char*)p, 1, fsize, infp) != fsize) {
		c_error("Unable to load file");
	}
	fclose(infp);
	*(p + fsize) = '\0';

	
	for(char *check = p; check < p + fsize; check++) {
		if(*check == '\0') {
			printf("Cannot compile because file %s contains NULL character(s)\n", file);
    		exit(0);
		}
	}
	return p;
}

int main(int argc, char const *argv[]) {
	char *file = (char*)argv[1];
	if(file != NULL) {
		printf("Attempting to compile %s\n", file);
		char *input = read_file_into_buffer(file);

		printf("Lexing %s\n", file);
		List tokens;
		lex(&tokens, input);

		printf("Parsing %s\n", file);

		Node *node = parse(&tokens);

		printf("Code Generation %s\n", file);

		generate(node);

		printf("Done %s\n", file);

		free(input);
		token_free(&tokens);
		
	} else {
		printf("No input file(s) specified\n");
	}
	
}