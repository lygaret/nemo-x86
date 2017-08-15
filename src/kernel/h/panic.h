#ifndef __kernel_panic_h
#define __kernel_panic_h

#include <stdlib/stdlib.h>

noreturn void kpanic(const char* restrict format, ...);

#endif
