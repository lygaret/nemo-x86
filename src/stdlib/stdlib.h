#ifndef __stdlib_h__
#define __stdlib_h__

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdnoreturn.h>

#define unused(x) (void)(x)

// string.c
int  parseint(const char * str, int base);
void printint(int n, int base, char outbuf[17]);

// mem.c
void memzero(void * dest, size_t n);
uintptr_t ptralign(uintptr_t ptr, size_t alignment);


#endif
