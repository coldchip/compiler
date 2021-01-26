#include "chipcode.h"

void c_error(char *format, ...) {
	va_list args;
    va_start(args, format);

	char fmt[1000];
	#ifdef __linux
		snprintf(fmt, sizeof(fmt), "\033[0;31m[ChipCode] %s\033[0m\n", format);
	#else
		snprintf(fmt, sizeof(fmt), "[ChipCode] %s\n", format);
	#endif
	vprintf(fmt, args);
    
    va_end(args);
	exit(1);
}

char *strmalloc (const char *s) {
  size_t len = strlen (s) + 1;
  void *new = malloc (len);
  if (new == NULL)
    return NULL;
  return (char *) memcpy (new, s, len);
}

char *read_file_into_buffer(char *file) {
	FILE *infp = fopen(file, "rb");
    if (!infp) {
    	printf("Cannot open %s\n", file);
    	exit(0);
    }
    fseek(infp, 0, SEEK_END);
	long fsize = ftell(infp);
	char *p = malloc(fsize + 1);
	fseek(infp, 0, SEEK_SET);

	if(fread((char*)p, 1, fsize, infp) != fsize) {
		c_error("Unable to load file");
	}
	fclose(infp);
	*(p + fsize) = '\0';

	
	for(char *check = p; check < p + fsize; check++) {
		if(*check == '\0') {
			printf("Cannot compile because file %s contains NULL character(s)\n", file);
    		exit(0);
		}
	}
	return p;
}