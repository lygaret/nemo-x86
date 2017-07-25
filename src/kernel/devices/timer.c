#include <stdlib/stdlib.h>
#include <kernel/h/screen.h>
#include <kernel/h/ioport.h>
#include <kernel/h/isr.h>
#include <kernel/h/timer.h>

uint32_t tick_count = 0;
uint32_t second_count = 0;

void timer_handler(isr_registers_t * reg) {
  tick_count++;
  if (tick_count % 1000 == 0) {
    second_count++;
    print('.');
  }
}

void timer_sleep(uint32_t ms) {
  uint32_t now = tick_count;
  ++ms;

  while (tick_count - now < ms) { ; }
}

void timer_install(uint16_t hz) {
  uint32_t divisor = PIT_BASE_HZ / hz;
  outb(PIT_COMMAND, PIT_COMMAND_INIT);
  outb(PIT_CHAN_0, divisor & 0xFF);
  outb(PIT_CHAN_0, divisor >> 8); 
  
  // irq 0 is the timer
  irq_set_handler(0, *timer_handler);
}
