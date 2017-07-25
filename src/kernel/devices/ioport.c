#include <stdlib.h>
#include <kernel/h/ioport.h>

inline void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline void outw(uint16_t port, uint16_t val) {
  asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

inline void outl(uint16_t port, uint32_t val) {
  asm volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline uint16_t inw(uint16_t port) {
  uint16_t ret;
  asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline uint32_t inl(uint16_t port) {
  uint32_t ret;
  asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

// force the cpu to wait a few cycles for an io operation to complete  
inline void io_wait(void) {
  asm volatile ("0: jmp 1f \n\t"
                "1: jmp 2f \n\t"
                "2:");
}
