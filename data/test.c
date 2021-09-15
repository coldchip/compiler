void test(int ptr) {
	int z = 0;
	int y = &z;
	*(y + 1) = 20;
	return;
}

int main() {
	/*
	int x = 2;
	int a = x;
	int b = a + x;
	int c = a + b + x;
	int d = a + b + c + x;
	int e = a + b + c + d + x;
	int f = a + b + c + d + e + x;
	int g = a + b + c + d + e + f + x;
	int h = a + b + c + d + e + f + g + x;
	int i = a + b + c + d + e + f + g + h + x;
	int j = a + b + c + d + e + f + g + h + i + x;
	int k = a + b + c + d + e + f + g + h + i + j + x;
	int l = a + b + c + d + e + f + g + h + i + j + k + x;
	int m = a + b + c + d + e + f + g + h + i + j + k + l + x;
	int o = a + b + c + d + e + f + g + h + i + j + k + l + m + x;
	int p = o * (o + o);
	*/

	int g = 99; 
	int h = 9999;
	
	int a = 6969;

	//while(a < 100) {
	int b = (&a) + 1;

	int c = *(b - 1);
	//}

}