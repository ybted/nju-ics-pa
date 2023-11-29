#include <am.h>

Area heap;

void putch(char ch) {
  char buffer[2];
  buffer[0] = ch;
  buffer[1] = '\0';
  printf("%s", buffer);
}

void halt(int code) {
  exit(code);
}
