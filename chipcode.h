#ifndef CHIPCODE_H
#define CHIPCODE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define VERSION 1.0
#define API extern

// chipcode.c

API void c_error(char *format, ...);

// macro.c

API char *macro(char *input);

#endif