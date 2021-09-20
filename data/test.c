
struct Object {
	
};

void sleep(int a) {
	__asm__("\tload 0\n\tsyscall 2");
}

void print(int a) {
	__asm__("\tload 0\n\tsyscall 0");
}

void print_float(float a) {
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

int test(int a, int b, int c) {
	return a + b + c;
}

int main() {
	
	int i = 0;

	float x = 0.1;

	int a = 0;
	int b = 0;
	int c = 0;

	while(i < 30) {
		
		int d = test(a, b, c);
		print(d);
		a = a + 1;
		b = b + 1;
		c = c + 1;

		i = i + 1;
		sleep(1);
	}
	
	print(sin(1/2));
	

	int z = sin(0.7);

	print_float(z);
}

