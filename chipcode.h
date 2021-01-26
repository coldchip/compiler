#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define VERSION 1.0

/* chipcode.c */

void c_error(char *format, ...);
char *strmalloc (const char *s);
char *read_file_into_buffer(char *file);

#endif