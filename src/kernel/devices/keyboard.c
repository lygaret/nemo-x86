#include <stdlib/stdlib.h>
#include <kernel/h/screen.h>
#include <kernel/h/ioport.h>
#include <kernel/h/keyboard.h>
#include <kernel/h/timer.h>

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	

uint8_t get_scancode() {
  uint8_t scancode = 0;
  do {
    if (inb(KEYBOARD_DATA) != scancode) {
      scancode = inb(KEYBOARD_DATA);
      if (scancode > 0)
        return scancode;
    }
  } while(1);
}

void keyboard_handler(isr_registers_t * reg) {
  uint8_t scancode = get_scancode();
  if (scancode & 0x80) {
    // key release
  } else {
    // key pressed
    char out[16];
    prints("seconds: ");
    printint(second_count, 10, out);
    prints(out);
    prints("    ticks: ");
    printint(tick_count, 10, out);
    prints(out);
    print('\n');
    //print(kbdus[scancode]);
  }
}

void keyboard_install() {
  // irq 1 is the keyboard
  irq_set_handler(1, *keyboard_handler);
}
