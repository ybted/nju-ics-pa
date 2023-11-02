def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  printf("DEBUG lui: %x\n", *ddest);
}

def_EHelper(addi) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  rtl_li(s, ddest, id_src1->imm + s->pc);
}