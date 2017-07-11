#include <stdlib/stdlib.h>
#include <kernel/kernel.h>
#include <core/console.h>

extern void _panic();
void kpanic(const char * restrict format, ...) {

  va_list args;
  va_start(args, format);

  attrcur(console_attr(BRIGHT_GRAY, RED));

  kprintf("PANIC! \n");
  kvprintf(format, args);

  va_end(args);
  _panic();
}
