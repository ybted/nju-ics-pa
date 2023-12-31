#include "../local-include/reg.h"
#include <cpu/ifetch.h>
#include "../include/isa-all-instr.h"
def_all_THelper();

static uint32_t get_instr(Decode *s) {
  return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)

static def_DopHelper(i) {
  op->imm = val;
}

static def_DopHelper(r) {
  bool is_write = flag;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &gpr(val);
}

static def_DHelper(I) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

static def_DHelper(U) {
  decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

static def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
  sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
  decode_op_i(s, id_src2, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, false);
}

static def_DHelper(J) {
  uint32_t target = 0;
  if (s->isa.instr.j.imm31) {
    target = 0xffe00000 | ( (s->isa.instr.j.imm21_30 + (s->isa.instr.j.imm31 << 19) + (s->isa.instr.j.imm20 << 10) + (s->isa.instr.j.imm12_19 << 11)) << 1);
  } else {
    target = (s->isa.instr.j.imm21_30 + (s->isa.instr.j.imm31 << 19) + (s->isa.instr.j.imm20 << 10) + (s->isa.instr.j.imm12_19 << 11)) << 1;
  }
  decode_op_i(s, id_src1, target, true);
  decode_op_r(s, id_dest, s->isa.instr.j.rd, true);
}

static def_DHelper(R) {
  decode_op_r(s, id_src1, s->isa.instr.r.rs1, false);
  decode_op_r(s, id_src2, s->isa.instr.r.rs2, false);
  // sword_t func3 = s->isa.instr.r.func3;
  // sword_t func7 = s->isa.instr.r.func7;
  decode_op_r(s, id_dest, s->isa.instr.r.rd, true);
}

static def_DHelper(B) {
  decode_op_r(s, id_src1, s->isa.instr.b.rs1, false);
  decode_op_r(s, id_src2, s->isa.instr.b.rs2, false);
  sword_t imm = s->isa.instr.b.imm31 << 12 | s->isa.instr.b.imm25_30 << 5 | s->isa.instr.b.imm7 << 11 | s->isa.instr.b.imm8_11 << 1;
  if (s->isa.instr.b.imm31) {
    imm = imm | 0xffffe000;
  } 
  decode_op_i(s, id_dest, imm, true);
  // printf("b_imm: %u\n", imm);
}

def_THelper(load) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", lh);
  def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", lhu);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", lbu);
  return EXEC_ID_inv;
}

def_THelper(store) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sh);
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", sb);
  return EXEC_ID_inv;
}

def_THelper(calculate) {
  def_INSTR_TAB("0000000 ????? ????? 000 ????? ????? ??", add);
  def_INSTR_TAB("0100000 ????? ????? 000 ????? ????? ??", sub);
  def_INSTR_TAB("0100000 ????? ????? 101 ????? ????? ??", sra);
  def_INSTR_TAB("0000000 ????? ????? 101 ????? ????? ??", srl);
  def_INSTR_TAB("0000000 ????? ????? 011 ????? ????? ??", sltu);
  def_INSTR_TAB("0000000 ????? ????? 100 ????? ????? ??", xor);
  def_INSTR_TAB("0000000 ????? ????? 110 ????? ????? ??", or);
  def_INSTR_TAB("0000000 ????? ????? 001 ????? ????? ??", sll);
  def_INSTR_TAB("0000000 ????? ????? 010 ????? ????? ??", slt);
  def_INSTR_TAB("0000000 ????? ????? 111 ????? ????? ??", and);
  def_INSTR_TAB("0000001 ????? ????? 000 ????? ????? ??", mul);
  def_INSTR_TAB("0000001 ????? ????? 001 ????? ????? ??", mulh);
  def_INSTR_TAB("0000001 ????? ????? 100 ????? ????? ??", div);
  def_INSTR_TAB("0000001 ????? ????? 101 ????? ????? ??", divu);
  def_INSTR_TAB("0000001 ????? ????? 110 ????? ????? ??", rem);
  def_INSTR_TAB("0000001 ????? ????? 111 ????? ????? ??", remu);
  return EXEC_ID_inv;
}

def_THelper(immop) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", addi);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??", sltiu);
  def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", andi);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", xori);
  def_INSTR_TAB("0100000 ????? ????? 101 ????? ????? ??", srai);
  def_INSTR_TAB("0000000 ????? ????? 101 ????? ????? ??", srli);
  def_INSTR_TAB("0000000 ????? ????? 001 ????? ????? ??", slli);
  return EXEC_ID_inv;
}

def_THelper(bop) {
  def_INSTR_TAB("?????? ????? ????? 000 ????? ????? ??", beq);
  def_INSTR_TAB("?????? ????? ????? 001 ????? ????? ??", bne);
  def_INSTR_TAB("?????? ????? ????? 101 ????? ????? ??", bge);
  def_INSTR_TAB("?????? ????? ????? 100 ????? ????? ??", blt);
  def_INSTR_TAB("?????? ????? ????? 110 ????? ????? ??", bltu);
  def_INSTR_TAB("?????? ????? ????? 111 ????? ????? ??", bgeu);
  
  return EXEC_ID_inv;
}

def_THelper(main) {
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I     , load);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S     , store);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", U     , lui);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00100 11", I     , immop);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U     , auipc);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J     , jal);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11001 11", I     , jalr);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01100 11", R     , calculate);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11000 11", B     , bop);
  def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);

  return table_inv(s);
};

int isa_fetch_decode(Decode *s) {
  s->isa.instr.val = instr_fetch(&s->snpc, 4);
  int idx = table_main(s);
  return idx;
}
