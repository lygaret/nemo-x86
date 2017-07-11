#ifndef __stdlib_h__
#define __stdlib_h__

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define unused(x) (void)(x)

// string.c
int atoi(const char * str, int base);
void itoa(int n, int base, char outbuf[17]);

// mem.c
void memzero(void * dest, size_t n);

#endif
