#include <cpu/cpu.h>

void sdb_mainloop();

void engine_start() {
#ifdef CONFIG_TARGET_AM
  cpu_exec(-1);
  printf("exec until end!\n");
#else
  /* Receive commands from user. */
  sdb_mainloop();
#endif
}
