def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
  //printf("DEBUG lw: %x\n", *ddest);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
  //printf("DEBUG sw: %x\n", *ddest);
}
