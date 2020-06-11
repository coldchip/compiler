#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
		parse(token);

		vm_exec();
		/*
		while(token != NULL) {
			printf("%s\n", token->data);
			token = token->next;
		}
		*/
		free(buf);
	} else {
		printf("No input file(s) specified\n");
	}
	
}