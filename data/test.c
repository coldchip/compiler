void printf(a) {
	__call__("printf", a);
}

void test(b) {
	__call__("test", b);
}

void main() {
	var cc = "aa";
	var dd = 2999;
	printf(cc);
	test(dd);
}