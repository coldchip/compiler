#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chipcode.h"

void print_hex(const char *string) {
	unsigned char *p = (unsigned char *) string;

	for (int i=0; i < 200; ++i) {
	        if (! (i % 8) && i) {
	            printf(" RW\n");
	        }

	        printf("%02x", p[i]);
	}
	printf("\n\n");
}