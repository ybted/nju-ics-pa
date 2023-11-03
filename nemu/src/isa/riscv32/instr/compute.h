def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  //printf("DEBUG lui: %x\n", *ddest);
}

def_EHelper(addi) {
  //printf("DEBUF addi %x %x\n", *id_src1->preg, id_src2->imm);
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
  //printf("DEBUG addi: %x\n", *ddest);
}

def_EHelper(sltiu) {
  rtl_subi(s, s0, dsrc1, id_src2->imm);
  if ((*s0) < 0) {
    rtl_addi(s, ddest, rz, 1);
  } else {
    rtl_addi(s, ddest, rz, 0);
  }
  printf("DEBUG sltiu: %u %u %u\n", *ddest, *(id_src1->preg), id_src2->imm);
}

def_EHelper(add) {
  // printf("DEBUG ADD: %x %x %x\n", *ddest, *id_src1->preg, *id_src2->preg);
  rtl_add(s, ddest, dsrc1, dsrc2);
  // printf("DEBUG ADD: %x %x %x\n", *ddest, *id_src1->preg, *id_src2->preg);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(auipc) {
  rtl_li(s, ddest, id_src1->imm + s->pc);
  //printf("DEBUG auipc: %x\n", *ddest);
}