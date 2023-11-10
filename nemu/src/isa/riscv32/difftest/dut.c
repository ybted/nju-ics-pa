#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  // int reg_num = ARRLEN(cpu.gpr);
  // for (int i = 0; i < reg_num; i++) {
  //   if (ref_r->gpr[i] != cpu.gpr[i]) {
  //     return false;
  //   }
  // }
  // if (ref_r->pc != cpu.pc) {
  //   return false;
  // }
  return true;
}

void isa_difftest_attach() {
}
