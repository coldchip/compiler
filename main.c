#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libgen.h>
#include "chipcode.h"
#include "macro.h"
#include "lex.h"
#include "parse.h"
#include "codegen.h"

int main(int argc, char const *argv[]) {
	char *file = (char*)argv[1];
	if(file != NULL) {
		char *file2 = strmalloc(file);
		char *dir = dirname(file2);
		printf("Attempting to compile %s\n", file);
		char *input = read_file_into_buffer(file);

		printf("Preprocessing\n");
		char *processed = preprocess(dir, input);

		printf("Lexing %s\n", file);
		List tokens;
		lex(&tokens, processed);

		printf("Parsing %s\n", file);

		Node *node = parse(&tokens);

		printf("Code Generation %s\n", file);

		generate(node);

		printf("Done %s\n", file);

		free(file2);
		free(input);
		free(processed);
		token_free(&tokens);
		
	} else {
		printf("No input file(s) specified\n");
	}
	
}