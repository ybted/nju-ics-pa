def_EHelper(csrrw) {
  // mtvec
  if (id_src2->imm == 0x305) {
    cpu.mtvec = *dsrc1;
  }
}