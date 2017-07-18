#include <system/h/console.h>
#include <system/h/panic.h>

extern void _panic();

void panic(const char * message) {
  attrcur(console_attr(BRIGHT_GRAY, RED));
  prints(message);

  _panic();
}
