void print_int(int a) {
	__asm__("\tload 0\n\tsyscall 0");
}

void print_float(float a) {
	__asm__("\tload 0\n\tsyscall 1");
}