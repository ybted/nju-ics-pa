#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(addi) f(nemu_trap) f(auipc) f(jal) f(jalr) f(add) f(sub) f(sltiu) f(beq) f(bne) f(sltu)
 
def_all_EXEC_ID();
