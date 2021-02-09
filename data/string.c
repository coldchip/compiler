char strjoin(char a, char b) {
	int s_a = strlen(a);
	int s_b = strlen(b);
	char[] c = [s_a + s_b + 1];

	int i_a = 0;
	int i_b = 0;
	int i_c = 0;
	while(i_a < s_a) {
		c[i_c] = a[i_a];
		i_a = i_a + 1;
		i_c = i_c + 1;
	}

	while(i_b < s_b) {
		c[i_c] = b[i_b];
		i_b = i_b + 1;
		i_c = i_c + 1;
	}

	return c;
}

char strjoinc(char a, char b) {
	int s_a = strlen(a);
	
	char[] c = [s_a + 1 + 1];

	int i_a = 0;
	int i_b = 0;
	int i_c = 0;
	while(i_a < s_a) {
		c[i_c] = a[i_a];
		i_a = i_a + 1;
		i_c = i_c + 1;
	}

	c[i_c] = b;

	return c;
}

int strlen(char str) {
	int i = 0;
	int max = sizeof(str);
	while(i < max) {
		if(str[i] == 0) {
			return i;
		}
		i = i + 1;
	}
	return i;
}

int strstr(char haystack, char needle) {
	int i = 0;
	while(i < strlen(haystack)) {
		char at = haystack[i];
		if(at == needle) {
			return 1;
		}
		i = i + 1;
	}
	return 0;
}

int strcmp(char a, char b) {
	int a_len = strlen(a);
	int b_len = strlen(b);
	if(a_len == b_len) {
		int i = 0;
		while(i < a_len) {
			if(a[i] != b[i]) {
				return 1;
			}
			i = i + 1;
		}
		return 0;
	}
	return 1;
}

char itos(int n) {
	return __callinternal__itos(n);
}

int sizeof(int arr) {
	return __callinternal__sizeof(arr);
}

void explode(char data, char delim, int y) {
	char[] result = [1000];
	int data_len = strlen(data);
	int i = 0;
	int f = 0;
	while(i < data_len) {
		int g = 0;
		char[] tmp = strjoinc(result, data[i]);
		result = tmp;
		i = i + 1;
		while(data[i] == delim[g]) {
			if(data[i] == 0) {
				return "";
			}
			if(delim[g] == 0) {
				return "";
			}
			i = i + 1;
			g = g + 1;
			if(g == strlen(delim)) {
				if(f == y) {
					return result;
				}
				char[] empty = "";
				result = empty;
				f = f + 1;
			}
		}
	}
	return "";
}