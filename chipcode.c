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