#include <stdlib/stdlib.h>

void memzero(void * dest, size_t n) {
  if (n) {
    char *d = dest;
    do {
      *d++ = 0;
    } while (--n);
  }
}

// round up to the nearest address `align`
uintptr_t ptralign(uintptr_t ptr, size_t align) {
  return (ptr + (align - 1)) & -align;
}
