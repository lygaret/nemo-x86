#include <stdlib/stdlib.h>

void reverse(char str[], int length) {
  char tmp;
  int start = 0;
  int end   = length - 1;

  while (start < end) {
    tmp = *(str + start);
    *(str+start) = *(str+end);
    *(str+end)   = tmp;

    start++;
    end--;
  }
}

void itoa(int n, int base, char * str) {
  int i = 0;

  // handle 0 explicitly
  if (n == 0) {
    str[i]   = '0';
    str[++i] = '\0';
    return;
  }

  while(n > 0) {
    str[i++] = "0123456789abcdef"[n % base];
    n = n / base;
  }

  str[i] = '\0';
}

int atoi(const char *str, int base) {
  unsigned int size = 0;
  unsigned int count = 1;
  signed int sign = 1;

  if (str[0] == '-') {
    sign = -1;
    str++;
  }
 
  for (int j = 0; str[j] != '\0'; j++, count *= base)
    size += ((str[j] - '0') * count);
 
  return size;
}
