#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Stack {
	void *mem;
	int sp;
} Stack;

Stack *new_stack() {
	Stack *stack = malloc(sizeof(Stack));
	stack->mem = malloc(1);
	stack->sp = 0;
	return stack;
}

void stack_add(Stack *stack, void* data, int size) {
	stack->mem = realloc(stack->mem, stack->sp + size);
	memcpy(stack->mem + stack->sp, data, size);
	stack->sp += size;
}

void stack_grow(Stack *stack, int size) {
	stack->mem = realloc(stack->mem, stack->sp + size);
	stack->sp += size;
}

int main(int argc, char const *argv[])
{
	/* code */
	char instructions[] = "pushstack\niload 12\niload H\niload e\niload l\niload l\niload o\niload v\niload W\niload o\niload r\niload l\niload d\niload !\ncall printf\npusharg\nexec\npopstack";
	

	Stack *stack = NULL;
	Stack *called_stack = NULL;

	char* ins_save = instructions; 
	char *token = strtok_r(instructions, "\n", &ins_save);
	while( token != NULL ) {
		char *ins = strtok_r(token, " ", &token);
		char *val = strtok_r(NULL, " ", &token);
		if(strcmp(ins, "pushstack") == 0) {
			stack = new_stack();
		}
		if(strcmp(ins, "iload") == 0) {
			stack_add(stack, val, sizeof(char));
		}
		if(strcmp(ins, "call") == 0) {
			if(strcmp(val, "printf") == 0) {
				called_stack = new_stack();
			} else {
				printf("Unknown call operation %s\n", val);
			}
		}
		if(strcmp(ins, "pusharg") == 0) {
			stack_grow(called_stack, stack->sp);
			memcpy(called_stack, stack->mem, stack->sp);
		}
		if(strcmp(ins, "exec") == 0) {
			printf("%s\n", (char*)called_stack->mem);
		}
		token = strtok_r(NULL, "\n", &ins_save);
	}
	return 0;
}