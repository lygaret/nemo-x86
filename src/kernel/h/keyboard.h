#include <stdlib/stdlib.h>
#include <kernel/h/isr.h>

#define KEYBOARD_DATA    0x60
#define KEYBOARD_COMMAND 0x64

void keyboard_handler(isr_registers_t *);
void keyboard_install();
