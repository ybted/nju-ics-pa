def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}


def_EHelper(ecall) {
  printf("ecall a7: %u\n", gpr(17));
  word_t epc = isa_raise_intr( gpr(17), cpu.pc + 4);
  rtl_j(s, epc);
}


def_EHelper(mret) {
  // printf("mret!!\n");
  // if (cpu.mcause == 1 || cpu.mcause == 2)
    rtl_j(s, cpu.mepc + 4);
  // else 
  //   rtl_j(s, cpu.mepc);
}