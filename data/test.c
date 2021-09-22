#include "io.c"
#include "socket.c"

float sin(float x) {
	float res = 0.0;
	float pow = x;
	float fact = 1.0;
	int i = 0;
	while(i < 5) {
		res = res+(pow/fact);
		pow = pow * ((0.0 - 1.0) * x * x);
		fact = fact * ((2.0 * (i + 1.0)) * (2.0 * (i + 1.0) + 1.0));
		i = i + 1;
	}
	return res;
}

void main() {
	float i = 0.0;

	while(i < 10.0) {
		print_float(sin(i));
		i = i + 0.2;
	}

	// char *v = "lolol";
}