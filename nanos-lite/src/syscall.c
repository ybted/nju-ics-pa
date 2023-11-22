#include <common.h>
#include "syscall.h"
#include "fs.h"
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
  c->GPRx = fs_write(fd, buf, count);
}

void sys_brk(Context *c) {
  c->GPRx = 0;
}

void sys_open(Context* c) {
  char* pathname = (char*)c->GPR2;
  int flags = (int)c->GPR3;
  int mode = (int)c->GPR4;
  c->GPRx = fs_open(pathname, flags, mode);
}

void sys_read(Context* c) {
  int fd = c->GPR2;
  void* buf = (void *)c->GPR3;
  int count = c->GPR4;
  c->GPRx = fs_read(fd, buf, count);
}

void sys_lseek(Context* c) {
  c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
}

void sys_close(Context* c) {
  c->GPRx = fs_close(c->GPR2);
}

void sys_gettimeofday(Context* c) {
  struct timeval *tv = (struct timeval* ) c->GPR2;
  __uint64_t time = io_read(AM_TIMER_UPTIME).us;
  tv->tv_usec = (time % 1000000);
  tv->tv_sec = (time / 1000000);
  c->GPRx = 0;
}


void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  // #ifdef CONFIG_STRACE
  printf("strace: %d\n", a[0]);
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
    case SYS_open:
      sys_open(c);
      break;
    case SYS_read:
      sys_read(c);
      break;
    case SYS_lseek:
      sys_lseek(c);
      break;
    case SYS_close:
      sys_close(c);
      break;
    case SYS_gettimeofday:
      sys_gettimeofday(c);
      break;
  }
}
