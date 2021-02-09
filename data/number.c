int stoi(char str) {
	int res = 0;
	int sign = 1;
	int i = 0;
	if(charat(str, 0) == '-') {
		sign = 2 - sign;
		i = i + 1;
	}
	while(i < strlen(str)) {
		res = res * 10 + charat(str, i) - '0';
		i = i + 1;
	}

	return sign * res;
}

int rand(int min, int max) {
	return __callinternal__rand(min, max);
}