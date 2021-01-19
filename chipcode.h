#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define VERSION 1.0
#define API extern

/* chipcode.c */

API void c_error(char *format, ...);

#endif