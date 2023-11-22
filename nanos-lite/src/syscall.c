#include <common.h>
#include "syscall.h"
void sys_yield(Context *c) {
  yield();
  c->GPRx = 0;
}

void sys_exit(Context *c) {
  printf("EXIT!\n");
  c->GPRx = 0;
  halt(0);
}

void sys_write(Context *c) {
  int fd = c->GPR2;
  void* buf = (void*)c->GPR3;
  int count = c->GPR4;
  if (fd == 1 || fd == 2) {
    for (int i = 0; i < count; i ++) {
      putch(*(char *)(buf+i));
    }
  }
  c->GPRx = count;
}

void sys_brk(Context *c) {
  c->GPRx = 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  // #ifdef CONFIG_STRACE
  // printf("strace: %d\n", a[0]);
  // #endif
  switch (a[0]) {
    case SYS_exit:
      sys_exit(c);
      break;
    case SYS_yield:
      sys_yield(c);
      break;
    case SYS_write:
      sys_write(c);
      break;
    case SYS_brk:
      sys_brk(c);
      break;
  }
}
