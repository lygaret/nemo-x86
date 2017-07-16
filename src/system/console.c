#include <stdlib/stdlib.h>
#include <system/system.h>

/// statically configured
static console_t console = {
  .buffer      = (uint16_t *)(0xe00b8000),
  .buf_width   = 80,
  .buf_height  = 25,
  .cur_attr    = console_attr(BRIGHT_GRAY, BLUE),
  .cur_clear   = ' ',
  .cur_x       = 0,
  .cur_y       = 0
};

void attrcur(console_attr_t attr) {
  console.cur_attr = attr;
}

void movecur(uint8_t x, uint8_t y) {
  console.cur_x = x;
  console.cur_y = y;
}

/// copy the character from xs, ys to xd, yd
void copyc(uint8_t sx, uint8_t sy, uint8_t dx, uint8_t dy) {
  console.buffer[dy * console.buf_width + dx] = console.buffer[sy * console.buf_width + sx];
}

/// print the given character at x, y, with the given attribute
void putc(uint8_t x, uint8_t y, console_attr_t attr, char c) {
  console.buffer[y * console.buf_width + x] = attr << 8 | c;
}

/// clear the screen
void cls() {
  for (uint8_t x = 0; x < console.buf_width; x++) {
    for (uint8_t y = 0; y < console.buf_height; y++) {
      putc(x, y, console.cur_attr, console.cur_clear);
    }
  }
}

/// not exported:
/// scroll the screen `lines` lines upward, fill with spaces
void scroll(uint8_t lines) {
  console.cur_y -= lines;
  
  for (uint8_t y = 0; y < console.cur_y; y++) {
    for (uint8_t x = 0; x < console.buf_width; x++) {
      copyc(x, y + lines, x, y);
    }
  }

  for (uint8_t y = console.cur_y; y < console.buf_height; y++) {
    for (uint8_t x = 0; x < console.buf_width; x++) {
      putc(x, y, console.cur_attr, console.cur_clear);
    }
  }
}

/// print the given character with the current console attributes
void print(char c) {

  switch (c) {
  case '\n':
    console.cur_x  = 0;
    console.cur_y += 1;
    break;

  default:
    putc(console.cur_x++, console.cur_y, console.cur_attr, c);
    if (console.cur_x >= console.buf_width) {
      console.cur_x  = 0;
      console.cur_y += 1;
    }
  }

  // scroll if required
  if (console.cur_y >= console.buf_height) {
    scroll(1);
  }

}

/// print the given string 
void prints(const char* s) {
  char c;
  size_t len = 0;

  while ((c = s[len++])) {
    print(c);
  }
}

/// print the string given, up to len characters
void printn(const char* s, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (s[i] == '\0')
      break;

    print(s[i]);
  }
}
