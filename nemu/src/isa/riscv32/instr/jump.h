def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  rtl_j(s, id_src1->imm);
}