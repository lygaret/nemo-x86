#include <stdlib/stdlib.h>
#include <kernel/h/screen.h>
#include <kernel/h/stdio.h>

/// printf
void kprintf(const char* restrict format, ...) {
  va_list args;

  va_start(args, format);
  kvprintf(format, args);
  va_end(args);
}

/// vprintf (so we can forward from panic)
void kvprintf(const char * restrict format, va_list args) {
  while (*format != '\0') {

    // regular ol' string (no escapes)
    if (format[0] != '%' || format[1] == '%') {
      if (format[0] == '%')
        format++;

      // go ahead and jump ahead to the next percent
      size_t amount = 1;
      while (format[amount] && format[amount] != '%')
        amount++;

      // print what we found
      printn(format, amount);

      // move forward and try again
      format += amount;
      continue;
    }

    const char * format_begun_at = format++;

    // a character from args
    if (*format == 'c') {
      format++;
      print((char) va_arg(args, int));
    }

    // a string from args
    else if (*format == 's') {
      format++;
      prints(va_arg(args, const char*));
    }
    
    // a boolean value from args
    else if (*format == 'B') {
      format++;

      const int  num = va_arg(args, int);
      if (num) {
        prints("true");
      } else {
        prints("false");
      }
    }

    // a hex number from args
    else if (*format == 'h') {
      format++;

      int  num = va_arg(args, int);
      char buf[17];
      printint(num, 16, buf);

      prints(buf);
    }

    // a hex number from args
    else if (*format == 'l') {
      format++;

      unsigned long long num = va_arg(args, unsigned long long);
      char buf[17];
      printint(num >> 32, 16, buf); // top 32bits
      prints(buf);
      printint(num & 0xFFFFFFFF, 16, buf); // bottom 32bits
      prints(buf);
    }

    // a binary number from args
    else if (*format == 'b') {
      format++;

      const int  num = va_arg(args, int);
      char buf[17];
      printint(num, 2, buf);

      prints(buf);
    }

    // a decimal number from args
    else if (*format == 'd') {
      format++;

      const int  num = va_arg(args, int);
      char buf[17];
      printint(num, 10, buf);

      prints(buf);
    }

    // unknown otherwise, just print me out
    else {
      format = format_begun_at;
      prints(format);
    }
  }
}
