#include <unistd.h>
#include <stdio.h>

int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 10000) {
      write(1, "Hello World from Navy-apps for the 3th time!\n", 46);
      j = 0;
    }
  }
  return 0;
}
