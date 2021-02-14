int add(int a, int b) {
	return a + b;
}

int main() {
	int a = 0;
	int b = 1;
	int next = 0;
	while(a < 999999999) {
		next = add(a, b);
		a = b;
		b = next;
		dbg(next);
	}
}