#include <stdlib/stdlib.h>

// ports

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

// commands

#define PIC_EOI         0x20 // eoi (end of interrupt) command code

// initialization commands

#define ICW1_INIT       0x10 // initialization
#define ICW1_ICW4       0x01 // icw4 (not) needed
#define ICW1_SINGLE     0x02 // single (cascade) mode
#define ICW1_INTERVAL4  0x04 // call address interval 4 (8)
#define ICW1_LEVEL      0x08 // level triggered (edge mode)

#define ICW4_8086       0x01 // 8086/88 mode
#define ICW4_AUTO       0x02 // auto (normal) eoi
#define ICW4_BUF_SLAVE  0x08 // buffered mode/slave
#define ICW4_BUF_MASTER 0x0c // buffered mode/master
#define ICW4_SFNM       0x10 // special fully nested (not)

void pic_remap(uint8_t);
void pic_sendeoi(uint8_t);
