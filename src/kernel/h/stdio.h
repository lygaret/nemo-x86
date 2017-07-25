#ifndef __kernel_stdio_h
#define __kernel_stdio_h

#include <stdlib/stdlib.h>

void kprintf(const char* restrict format, ...);
void kvprintf(const char* restrict format, va_list argp);

#endif
