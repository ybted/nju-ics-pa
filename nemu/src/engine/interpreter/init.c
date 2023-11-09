#include <cpu/cpu.h>

void sdb_mainloop();

void engine_start() {
#ifdef CONFIG_TARGET_AM
  cpu_exec(-1);
  assert(0);
#else
  /* Receive commands from user. */
  sdb_mainloop();
#endif
}
