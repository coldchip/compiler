int strlen(string str) {
	return __callinternal__strlen(str);
}

string charat(string str, int index) {
	return __callinternal__charat(str, index);
}

int strstr(string haystack, char needle) {
	int i = 0;
	while(i < strlen(haystack)) {
		char at = charat(haystack, i);
		if(at == needle) {
			return 1;
		}
		i = i + 1;
	}
	return 0;
}

int strcmp(string a, string b) {
	int a_len = strlen(a);
	int b_len = strlen(b);
	if(a_len == b_len) {
		int i = 0;
		while(i < a_len) {
			if(charat(a, i) != charat(b, i)) {
				return 1;
			}
			i = i + 1;
		}
		return 0;
	}
	return 1;
}

string itos(int n) {
	return __callinternal__itos(n);
}