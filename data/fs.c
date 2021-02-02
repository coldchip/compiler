int fopen(string file, string mode) {
	return __callinternal__fopen(file, mode);
}

string fread(int fp, int size) {
	return __callinternal__fread(fp, size);
}

void fclose(int fd) {
	__callinternal__fclose(fd);
}