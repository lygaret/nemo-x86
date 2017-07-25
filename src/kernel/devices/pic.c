#include <kernel/h/screen.h>
#include <kernel/h/ioport.h>
#include <kernel/h/pic.h>

// master PIC vectors become offset1...offset1+7
// slave  PIC vectors become offset1+8...offset1+15
void pic_remap(uint8_t offset1) {

  // initialization sequence
  outb(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);
  outb(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
  io_wait();

  // icw2: vector offsets
  outb(PIC1_DATA, offset1);
  outb(PIC2_DATA, offset1 + 8);
  io_wait();
  
  // icw3: master/slave wiring
  outb(PIC1_DATA, 0b0100); // master has a slave at IRQ2
  outb(PIC2_DATA, 0b0010); // slave has cascade identity
  io_wait();

  // icw4: environment info
  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);
  io_wait();

  // null out data registers
  outb(PIC1_DATA, 0);
  outb(PIC2_DATA, 0);
  io_wait();
}

// send an EOI, to both master/slave if required
void pic_sendeoi(uint8_t irq) {
  outb(PIC1_COMMAND, PIC_EOI);
  if (irq >= 8)
    outb(PIC2_COMMAND, PIC_EOI);

  io_wait();
}
