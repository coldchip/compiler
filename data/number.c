int stoi(string str) {
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