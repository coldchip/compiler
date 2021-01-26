#include "string.c"

int main() {
	string motd = "Fibonacci number";
	int len = strlen(motd);
	int i = 0;
	while(i < len) {
		printf(charat(motd, i));
		i = i + 1;
	}

	int f = len;
	while(f > 0) {
		printf(charat(motd, f));
		f = 1 - f;
	}

	int o = 0;
	while(o < 10) {
		int a = 0;
		int b = 1;
		int c = 0;
		while(a < 10000000000) {
			int g = 1;
			while(g < 100000) {
				g = g + 1;
			}
			c = a + b;
			a = b;
			b = c;
			printf(a);
		}
		o = o + 1;
	}

	string done = "DONE";

	printf(done);
}