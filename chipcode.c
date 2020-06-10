#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "chipcode.h"

void c_error(char *format, ...) {
	va_list args;
    va_start(args, format);

	char fmt[1000];
	snprintf(fmt, sizeof(fmt), "[ChipCode] %s\n", format);
	vprintf(fmt, args);
    
    va_end(args);
	exit(1);
}