#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
    case 0:
      printf("EXIT!\n");
      c->GPRx = 0;
      break;
    case 1:
      yield();
      c->GPRx = 0;
      break;
  }
}
