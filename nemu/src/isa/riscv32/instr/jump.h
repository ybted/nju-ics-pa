def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  printf("DEBUG jal: %x %x\n", id_src1->imm, s->pc);
  rtl_j(s, id_src1->imm + s->pc);
}