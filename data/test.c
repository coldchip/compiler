int add(int a, int b) {
	return a + b;
}

int main() {
	int a = 0;
	int b;
	while(a < 999) {
		dbg(b);
		b = a = (a + 1);
	}
}