def_EHelper(csrrw) {
  // mtvec
  if (id_src2->imm == 0x305) {
    *ddest = cpu.mtvec; 
    cpu.mtvec = (*dsrc1);
  } else if (id_src2->imm == 0x300) {
    *ddest = cpu.mstatus;
    cpu.mstatus = (*dsrc1);
  } else if (id_src2->imm == 0x341) {
    *ddest = cpu.mepc;
    cpu.mepc = (*dsrc1);
  } else if (id_src2->imm == 0x342) {
    *ddest = cpu.mcause;
    cpu.mcause = (*dsrc1);
  } else assert(0);
}

def_EHelper(csrrs) {
  if (id_src2->imm == 0x305) {
    *ddest = cpu.mtvec; 
    cpu.mtvec = cpu.mtvec | (*dsrc1);
  } else if (id_src2->imm == 0x300) {
    *ddest = cpu.mstatus;
    cpu.mstatus = cpu.mstatus | (*dsrc1);
  } else if (id_src2->imm == 0x341) {
    *ddest = cpu.mepc;
    cpu.mepc = cpu.mepc | (*dsrc1);
  } else if (id_src2->imm == 0x342) {
    *ddest = cpu.mcause;
    cpu.mcause = cpu.mcause | (*dsrc1);
  } else assert(0);

}