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

void printint(int n, int base, char * str) {
  int i = 0;

  // handle 0 explicitly
  if (n == 0) {
    str[i++]   = '0';
  }
  
  while(n > 0) {
    str[i++] = "0123456789abcdef"[n % base];
    n = n / base;
  }

  if (base == 16) {
    while ((i % 8) > 0) {
        str[i++] = '0';
    }
  }

  reverse(str, i);
  str[i] = '\0';
}

int parseint(const char *str, int base) {
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
