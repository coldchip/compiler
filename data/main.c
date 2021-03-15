#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define STACK8 (x) ( *(uint8_t *)(stack + x) ) // Casts stack into 8  bits
#define STACK16(x) ( *(uint16_t*)(stack + x) ) // Casts stack into 16 bits
#define STACK32(x) ( *(uint32_t*)(stack + x) ) // Casts stack into 32 bits
#define STACK64(x) ( *(uint64_t*)(stack + x) ) // Casts stack into 64 bits

#define ENTRY(x) { \
	goto x; \
}

#define PUSH32(x) { \
	STACK32(sp) = x; \
	sp += 4; \
}

#define POP32(x) { \
	sp -= 4; \
	x = STACK32(sp); \
}

#define ADD32(x, y) { \
	x += y; \
}

#define MOV32(x, y) { \
	x = y; \
}

#define CMP(x, y) { \
	if(x == y) \
		cmp_flag = 0; \
	else if(x > y) \
		cmp_flag = 1; \
	else if(x < y) \
		cmp_flag = -1; \
}

#define GOTO(x) { \
	goto x; \
}

#define JE(x) { \
	if(cmp_flag == 0) \
		goto x; \
}

#define JG(x) { \
	if(cmp_flag == 1) \
		goto x; \
}

#define JL(x) { \
	if(cmp_flag == -1) \
		goto x; \
}

int main(int argc, char const *argv[]) {
	char stack[65535];

	uint64_t r0 = 0;
	uint64_t r1 = 0;
	uint64_t r2 = 0;
	uint64_t r3 = 0;
	uint64_t r4 = 0;
	uint64_t sp = 0;
	uint64_t fp = 0;
	int cmp_flag = 0;

	// Include asm
	#include "code.S"

	return 0;
}