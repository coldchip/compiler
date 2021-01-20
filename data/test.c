void add(int a, int b) {
	return a * b;
}

int main() {
	int a = 2;
	int b = 2;
	print(add(a, add(a, add(a, add(a, add(a, add(a, add(a, add(a, b)))))))));
}