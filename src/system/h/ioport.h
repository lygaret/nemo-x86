#include <stdlib/stdlib.h>

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outl(uint16_t port, uint32_t val);

uint8_t  inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

// force the cpu to wait a few cycles for an io operation to complete  
void io_wait(void);
