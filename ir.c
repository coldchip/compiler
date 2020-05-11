// ChipCode Intermediate Representation

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "chipcode.h"

IR *ir_init() {
	IR *ir = malloc(sizeof(IR));
	return ir;
}

void ir_add_instruction(IR *ir, char *format, ...) {
	va_list args;
    va_start(args, format);
    int bufsz = vsnprintf(NULL, 0, format, args);
    char buf[bufsz + 1];
    vsprintf((char*)&buf, format, args);

    printf("%s\n", buf);

    va_end(args);
}