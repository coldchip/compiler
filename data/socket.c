int socket(int family, int type, int protocol) {
	__asm__("\tload 0\n\tload 4\n\tload 8\n\tsyscall 2");
}