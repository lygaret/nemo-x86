#include <stdlib/stdlib.h>
#include <kernel/h/screen.h>

/// statically configured
static screen_t screen = {
  .buffer      = (uint16_t *)(0xe00b8000),
  .buf_width   = 80,
  .buf_height  = 25,
  .cur_attr    = screen_attr(BRIGHT_GRAY, BLUE),
  .cur_clear   = ' ',
  .cur_x       = 0,
  .cur_y       = 0
};

void attrcur(screen_attr_t attr) {
  screen.cur_attr = attr;
}

void movecur(uint8_t x, uint8_t y) {
  screen.cur_x = x;
  screen.cur_y = y;
}

/// copy the character from xs, ys to xd, yd
void copyc(uint8_t sx, uint8_t sy, uint8_t dx, uint8_t dy) {
  screen.buffer[dy * screen.buf_width + dx] = screen.buffer[sy * screen.buf_width + sx];
}

/// print the given character at x, y, with the given attribute
void putc(uint8_t x, uint8_t y, screen_attr_t attr, char c) {
  screen.buffer[y * screen.buf_width + x] = attr << 8 | c;
}

/// clear the screen
void cls() {
  for (uint8_t x = 0; x < screen.buf_width; x++) {
    for (uint8_t y = 0; y < screen.buf_height; y++) {
      putc(x, y, screen.cur_attr, screen.cur_clear);
    }
  }
}

/// not exported:
/// scroll the screen `lines` lines upward, fill with spaces
void scroll(uint8_t lines) {
  screen.cur_y -= lines;
  
  for (uint8_t y = 0; y < screen.cur_y; y++) {
    for (uint8_t x = 0; x < screen.buf_width; x++) {
      copyc(x, y + lines, x, y);
    }
  }

  for (uint8_t y = screen.cur_y; y < screen.buf_height; y++) {
    for (uint8_t x = 0; x < screen.buf_width; x++) {
      putc(x, y, screen.cur_attr, screen.cur_clear);
    }
  }
}

/// print the given character with the current screen attributes
void print(char c) {

  switch (c) {
  case '\n':
    screen.cur_x  = 0;
    screen.cur_y += 1;
    break;

  default:
    putc(screen.cur_x++, screen.cur_y, screen.cur_attr, c);
    if (screen.cur_x >= screen.buf_width) {
      screen.cur_x  = 0;
      screen.cur_y += 1;
    }
  }

  // scroll if required
  if (screen.cur_y >= screen.buf_height) {
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
