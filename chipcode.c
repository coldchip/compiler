#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "chipcode.h"

void c_error(char *format, ...) {
	va_list args;
    va_start(args, format);

	char fmt[1000];
	snprintf(fmt, sizeof(fmt), "\033[0;31m[ChipCode] %s\033[0m\n", format);
	vprintf(fmt, args);
    
    va_end(args);
	exit(1);
}