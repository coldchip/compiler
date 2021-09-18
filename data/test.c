
void print(int a) {
	__asm__("\tload 0\n\tsyscall 0");
}

void test(int a) {
	*a = (*a) + 1;
}

int main() {
	int i = 0;
	int a = 1;
	while(i < 10) {
		

		print(a);
		
		i = i + 1;
	}
}