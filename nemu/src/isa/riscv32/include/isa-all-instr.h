#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(bltu) f(srl) f(sra) f(divu) f(mulh) f(lhu) f(lh) f(blt) f(slt) f(div) f(rem) f(remu) f(mul) f(bgeu) f(slli) f(lui) f(lw) f(lbu) f(srai) f(sb) f(sw) f(sh) f(inv) f(addi) f(srli) f(nemu_trap) f(auipc) f(jal) f(jalr) f(add) f(sub) f(sll) f(and) f(sltiu) f(andi) f(xori) f(beq) f(bne) f(bge) f(sltu) f(xor) f(or) 
 
def_all_EXEC_ID();
