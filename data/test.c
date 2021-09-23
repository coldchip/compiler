#include "mem.c"
#include "io.c"
#include "socket.c"

float sin(float x) {
	float res = 0.0;
	float pow = x;
	float fact = 1.0;
	int i = 0;
	while(i < 5) {
		res = res+(pow/fact);
		pow = pow * ((0 - 1) * x * x);
		fact = fact * ((2 * (i + 1)) * (2 * (i + 1) + 1));
		i = i + 1;
	}
	return res;
}

struct Test {
	
};

void main() {
	float i = 0;

	while(1) {
		print_float(sin((float)i));
		i = i + 0.001;
	}
}