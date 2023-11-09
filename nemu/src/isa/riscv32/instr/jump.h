def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  //printf("DEBUG jal: %x %x\n", id_src1->imm, s->pc);
  rtl_j(s, id_src1->imm + s->pc);
  #ifdef CONFIG_FTRACE
  printf("JAL: %x\n", id_src1->imm + s->pc);
  printf("Num:       Value       Size Name\n");
  for (int i = 0; i < 1024; ++i) {
    if(elf_func[i].func_name[0] == '\0') {
      break;
    }
    printf("%3d: %016lx %5ld %s\n", i, elf_func[i].start, elf_func[i].size, elf_func[i].func_name);
  }
  #endif

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
  if ((int32_t)*dsrc1 >= (int32_t)*dsrc2) {
    rtl_j(s, id_dest->imm + s->pc);
  }
}

def_EHelper(blt) {
  if ((int32_t)*dsrc1 < (int32_t)*dsrc2) {
    rtl_j(s, id_dest->imm + s->pc);
  }
}

def_EHelper(bgeu) {
  if (*dsrc1 >= *dsrc2) {
    rtl_j(s, id_dest->imm + s->pc);
  }
}

def_EHelper(bltu) {
  if (*dsrc1 < *dsrc2) {
    rtl_j(s, id_dest->imm + s->pc);
  }
}


