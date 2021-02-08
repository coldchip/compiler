#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


// Allocates RWX memory of given size and returns a pointer to it. On failure,
// prints out the error and returns NULL.
void* alloc_executable_memory(size_t size) {
	void* ptr = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == (void*)-1) {
		perror("mmap");
		return NULL;
	}
	return ptr;
}

void emit_code_into_memory(unsigned char* m) {
	unsigned char code[] = {
		0x89, 0xf8,                   // mov %rdi, %rax
		0x48, 0x83, 0xec, 0x08 ,            // imul $4, %rax
		0x6b, 0xc0, 0xdf,             // imul $4, %rax
		0xc3                          // ret
	};
	memcpy(m, code, sizeof(code));
}

const size_t SIZE = 1024;
typedef long (*JittedFunc)(long);

// Allocates RWX memory directly.
int main() {
	void* m = alloc_executable_memory(SIZE);
	emit_code_into_memory(m);

	JittedFunc func = m;
	int result = func(200);
	printf("result = %d\n", result);
	return 0;
}