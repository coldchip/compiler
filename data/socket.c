int socket(int family, int type, int protocol) {
	__asm__("\tloadi 0\n\tloadi 4\n\tloadi 8\n\tsyscall 2");
}