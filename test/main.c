#include <core/core.h>

typedef union accessbyte {
  uint8_t      byte;
  gdt_access_t access;
} accessbyte_t;

int main() {
  accessbyte_t ab;
  ab.access.present    = 1;
  ab.access.privilege  = 0;
  ab.access.always_1   = 1;
  ab.access.executable = 1;
  ab.access.dirconf    = 1;
  ab.access.readwrite  = 00;
  ab.access.accessed   = 1;

  return ab.byte;
}
