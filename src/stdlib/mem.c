#include <stdlib/stdlib.h>

void memzero(void * dest, size_t n) {
  if (n) {
    char *d = dest;
    do {
      *d++ = 0;
    } while (--n);
  }
}
