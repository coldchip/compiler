int add(int a, int b) {
	return a + b;
}

int main() {
	int a = 0;
	int b = 1;
	int c = 0;
	while(1) {
		c = add(a, b);
		a = b;
		b = c;
		printf(a);
	}
}