#include <stdlib/stdlib.h>

typedef uint8_t screen_attr_t;
typedef enum {
    BLACK  = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GRAY,
    BRIGHT_BLACK,
    BRIGHT_BLUE,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    BRIGHT_BROWN,
    BRIGHT_GRAY,
} screen_color_t;

typedef struct {
  bool           initialized;
  uint16_t*      buffer;
  uint8_t        buf_width, buf_height;
  screen_attr_t cur_attr;
  uint16_t       cur_clear;
  uint8_t        cur_x, cur_y;
} screen_t;

/// return an attribute byte for the given colors
#define screen_attr(fg, bg) ((fg & 0x0F) | (bg & 0x0F) << 4)

/// set the cursor attribute
void attrcur(screen_attr_t);
void movecur(uint8_t x, uint8_t y);

/// clear the screen
void cls();

/// print the given character at x, y, with the given attribute
void putc(uint8_t x, uint8_t y, screen_attr_t, char);

/// print with the current character attributes and cursor
void print(char);
void prints(const char* s);
void printn(const char* s, size_t len);
