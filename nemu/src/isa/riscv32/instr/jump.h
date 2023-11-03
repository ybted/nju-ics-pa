def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  //printf("DEBUG jal: %x %x\n", id_src1->imm, s->pc);
  rtl_j(s, id_src1->imm + s->pc);
}

def_EHelper(jalr) {
  rtl_li(s, ddest, s->snpc);
  //printf("DEBUF jalr: %x %x %x\n", *id_src1->preg, id_src2->imm, s->pc);
  rtl_j(s,  *id_src1->preg);
}

def_EHelper(beq) {
  // printf("DEBUG beq: %u %x\n", id_dest->imm, id_dest->imm + s->pc);
  if (*dsrc1 == *dsrc2) {
    rtl_j(s, id_dest->imm + s->pc);
  }
}

def_EHelper(bne) {
  // printf("DEBUG bne: %u %x\n", id_dest->imm, id_dest->imm + s->pc);
  if (*dsrc1 != *dsrc2) {
    rtl_j(s, id_dest->imm + s->pc);
  }
}

def_EHelper(bge) {
  if (*dsrc1 >= *dsrc2) {
    rtl_j(s, id_dest->imm + s->pc);
  }
}
