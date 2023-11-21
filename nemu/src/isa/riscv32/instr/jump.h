int space = 0;
def_EHelper(jal) {
  #ifdef CONFIG_FTRACE
  for (int i = 0; i < 1024; ++i) {
    if(elf_func[i].func_name[0] == '\0') {
      break;
    }
    if (elf_func[i].start == id_src1->imm + s->pc) {
      for (int i = 0; i < space; i ++)
        printf(" ");
      printf("%x: call [%s@0x%lx]\n", s->pc, elf_func[i].func_name, elf_func[i].start);
      space += 2;
    }
  }
  #endif
  rtl_addi(s, ddest, &s->pc, 4);
  rtl_addi(s, &s->dnpc, &s->pc, id_src1->imm);

}

def_EHelper(jalr) {
  rtl_addi(s, s0, &s->pc, 4);
  rtl_addi(s, &s->dnpc, dsrc1, id_src2->imm);
  rtl_andi(s, &s->dnpc, &s->dnpc, ~1);
  rtl_addi(s, ddest, s0, 0);
  #ifdef CONFIG_FTRACE
  for (int i = 0; i < 1024; ++i) {
    if(elf_func[i].func_name[0] == '\0') {
      break;
    }
    if (elf_func[i].start == *id_src1->preg) {
      for (int i = 0; i < space; i ++)
        printf(" ");
      printf("%x: call [%s@0x%lx]\n", s->pc, elf_func[i].func_name, elf_func[i].start);
      space += 2;
    }
  }
  if (s->isa.instr.i.rs1 == 1 && s->isa.instr.i.rd == 0)
  {
    space -= 2;
    for (int i = 0; i < space; i ++)
      printf(" ");
    for (int i = 0; i < 1024; ++i) {
      if(elf_func[i].func_name[0] == '\0') {
        break;
      }
      if (elf_func[i].start <= *id_src1->preg && *id_src1->preg <= elf_func[i].start + elf_func[i].size) {
       
        printf("%x: ret [%s]\n", s->pc, elf_func[i].func_name);
      }
    }
    
  }
    
  #endif
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

