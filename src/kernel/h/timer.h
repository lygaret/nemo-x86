#ifndef __kernel_timer_h
#define __kernel_timer_h

#include <stdlib/stdlib.h>
#include <kernel/h/isr.h>

#define PIT_CHAN_0  0x40
#define PIT_CHAN_1  0x41
#define PIT_CHAN_2  0x42
#define PIT_COMMAND 0x43

// command byte:
// [channel : 2][rw : 2][mode : 3][bcd : 1]
// [00 - chan 0][11 : write high and low bytes][010 - mode 2 (square wave)][0 - straight 16bit count]

#define PIT_BASE_HZ       1193180    // hz divisor
#define PIT_COMMAND_INIT  0b00110100

uint32_t tick_count;
uint32_t second_count;

void timer_install(uint16_t hz);
void timer_sleep(uint32_t ms);

#endif
