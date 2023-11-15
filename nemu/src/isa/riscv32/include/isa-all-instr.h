#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) \
  f(lui) f(addi) f(sltiu) f(slti) f(andi) f(xori) f(ori) f(srai) f(srli) f(slli)\
  f(add) f(sub) f(mul) f(mulh) f(mulhu) f(div) f(divu) f(rem) f(remu) f(slt) f(sltu) f(xor) f(or) f(sll) f(sra) f(srl) f(and) f(auipc)\
  f(jal) f(jalr) f(beq) f(bne) f(bge) f(blt) f(bgeu) f(bltu)\
  f(lb) f(lh) f(lw) f(lbu) f(lhu) f(sb) f(sh) f(sw)\
  f(inv) f(nemu_trap)\
  f(ecall)
 

def_all_EXEC_ID();
