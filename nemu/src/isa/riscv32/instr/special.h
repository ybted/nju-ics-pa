def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}


def_EHelper(ecall) {
  word_t epc = isa_raise_intr( 1, cpu.pc);
  rtl_j(s, epc);
}


def_EHelper(mret) {
  // printf("mret!!\n");
  // if (cpu.mcause == 1 || cpu.mcause == 2)
    rtl_j(s, cpu.mepc);
  // else 
  //   rtl_j(s, cpu.mepc);
}