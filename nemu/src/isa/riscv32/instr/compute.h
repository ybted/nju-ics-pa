def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  //printf("DEBUG lui: %x\n", *ddest);
}

def_EHelper(addi) {
  //printf("DEBUF addi: %u %u %u\n", *ddest, *dsrc1, id_src2->imm);
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
  //printf("DEBUF addi: %u %u %u\n", *ddest, *dsrc1, id_src2->imm);
}

def_EHelper(andi) {
  rtl_andi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(xori) {
  rtl_xori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(ori) {
  rtl_ori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(srai) {
  rtl_srai(s, ddest, dsrc1, id_src2->imm  & 0b111111u);
}

def_EHelper(srli) {
  rtl_srli(s, ddest, dsrc1, id_src2->imm  & 0b111111u);
}

def_EHelper(slli) {
  rtl_slli(s, ddest, dsrc1, id_src2->imm  & 0b111111u);
}


def_EHelper(sltiu) {
  rtl_subi(s, s0, dsrc1, id_src2->imm);
  // printf("DEBUG sltiu: %u %u\n", *dsrc1, id_src2->imm);

  if ((*s0) > *dsrc1) {
    rtl_addi(s, ddest, rz, 1);
  } else {
    rtl_addi(s, ddest, rz, 0);
  }
  // printf("DEBUG sltiu: %u %u %u\n", *ddest, *dsrc1, id_src2->imm);
}

def_EHelper(slti) {
  rtl_subi(s, s0, dsrc1, id_src2->imm);
  // printf("DEBUG sltiu: %u %u\n", *dsrc1, id_src2->imm);

  if ((int32_t)(*s0) > (int32_t)*dsrc1) {
    rtl_addi(s, ddest, rz, 1);
  } else {
    rtl_addi(s, ddest, rz, 0);
  }
  // printf("DEBUG sltiu: %u %u %u\n", *ddest, *dsrc1, id_src2->imm);
}




def_EHelper(add) {
  // printf("DEBUG ADD: %x %x %x\n", *ddest, *id_src1->preg, *id_src2->preg);
  rtl_add(s, ddest, dsrc1, dsrc2);
  // printf("DEBUG ADD: %x %x %x\n", *ddest, *id_src1->preg, *id_src2->preg);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mul) {
  rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulh) {
  rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulhu) {
  rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(div) {
  rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(divu) {
  rtl_divu_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem) {
  rtl_divs_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(remu) {
  rtl_divu_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
}

def_EHelper(sltu) {
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
}

def_EHelper(xor) {
  rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or) {
  rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sll) {
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra) {
  rtl_sra(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srl) {
  rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and) {
  rtl_and(s, ddest, dsrc1, dsrc2);
}

def_EHelper(auipc) {
  rtl_li(s, ddest, id_src1->imm + s->pc);
  //printf("DEBUG auipc: %x\n", *ddest);
}