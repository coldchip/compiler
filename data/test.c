void printf() {
	int jj = 10;
}

int main() {
	int x = 10;
	int a = x;
	int b = a + x;
	int c = a + b + x;
	int d = a + b + c + x;
	int e = a + b + c + d + x;
	int f = a + b + c + d + e + x;
	int g = a + b + c + d + e + f + x;
	printf("%i\n", g);
	int h = a + b + c + d + e + f + g + x;
	int i = a + b + c + d + e + f + g + h + x;
	int j = a + b + c + d + e + f + g + h + i + x;
	int k = a + b + c + d + e + f + g + h + i + j + x;
	int l = a + b + c + d + e + f + g + h + i + j + k + x;
	int m = a + b + c + d + e + f + g + h + i + j + k + l + x;
	int o = a + b + c + d + e + f + g + h + i + j + k + l + m + x;
}