def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  printf("DEBUG: %x\n", id_src1->imm);
  printf("DEBUG: %x\n", s->pc);
  
  rtl_j(s, id_src1->imm + s->pc + 10000000000000000);
}