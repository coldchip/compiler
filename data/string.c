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

int sizeof(int arr) {
	return __callinternal__sizeof(arr);
}

char string_to_array(string str) {
	return __callinternal__string_to_array(str);
}

string array_to_string(int byte) {
	return __callinternal__array_to_string(byte);
}

void explode(string data, string delim, int y) {
	string result = "";
	int data_len = strlen(data);
	int i = 0;
	int f = 0;
	while(i < data_len) {
		int g = 0;
		string tmp = result + charat(data, i); // please implement casts -.-
		result = tmp;
		i = i + 1;
		while(charat(data, i) == charat(delim, g)) {
			i = i + 1;
			g = g + 1;
			if(g == strlen(delim)) {
				if(f == y) {
					return result;
				}
				string empty = "";
				result = empty;
				f = f + 1;
			}
		}
	}
	return "";
}