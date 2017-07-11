#ifndef __kernel_h__
#define __kernel_h__

#include <stdlib/stdlib.h>

void kpanic(const char* restrict format, ...);

void kprintf(const char* restrict format, ...);
void kvprintf(const char* restrict format, va_list argp);

#endif
