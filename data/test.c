int main() {
	int o = 0;
	while(o < 100) {
		printf(o);
		int a = 0;
		int b = 1;
		int c = 0;
		while(a < 100000) {
			c = a + b;
			a = b;
			b = c;
			printf(a);
		}
		o = o + 1;
	}
}