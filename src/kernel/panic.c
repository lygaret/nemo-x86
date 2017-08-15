#include <stdlib/stdlib.h>
#include <kernel/h/stdio.h>
#include <kernel/h/screen.h>

extern noreturn void _panic();

void kpanic(const char * restrict format, ...) {
  va_list args;
  va_start(args, format);

  attrcur(screen_attr(BRIGHT_GRAY, RED));

  kprintf("PANIC! \n");
  kvprintf(format, args);

  va_end(args);
  _panic();
}
