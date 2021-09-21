void print_float(int a) {
	__asm__("\tload 0\n\tsyscall 1");
}

float sin(float x) {
	float res = 0;
	float pow = x;
	float fact = 1;
	int i = 0;
	while(i < 5) {
		res = res+(pow/fact);
		pow = pow * ((0 - 1) * x * x);
		fact = fact * ((2*(i+1))*(2*(i+1)+1));
		i = i + 1;
		//print_float(fact);
	}
	return res;
}

void main() {
	int a = sin(2) * 10;
	print_float(a);
}