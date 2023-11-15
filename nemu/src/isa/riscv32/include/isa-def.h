#ifndef __ISA_RISCV32_H__
#define __ISA_RISCV32_H__

#include <common.h>

typedef struct {
  struct {
    rtlreg_t _32;
  } gpr[32];
  vaddr_t mepc;
  vaddr_t mstatus;
  vaddr_t mcause;
  vaddr_t mtvec;
  vaddr_t pc;
} riscv32_CPU_state;

// decode
typedef struct {
  union {
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      int32_t  simm11_0  :12;
    } i;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm4_0    : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      int32_t  simm11_5  : 7;
    } s;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm31_12  :20;
    } u;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm12_19  : 8;
      uint32_t imm20     : 1;
      uint32_t imm21_30  : 10;
      uint32_t imm31     : 1;
    } j;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcpde6_2 : 5;
      uint32_t rd        : 5;
      uint32_t func3     : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t func7     : 7;
    } r;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcpde6_2 : 5;
      uint32_t imm7      : 1;
      uint32_t imm8_11   : 4;
      uint32_t func3     : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t imm25_30  : 6;
      uint32_t imm31     : 1;
    } b;
    uint32_t val;
  } instr;
} riscv32_ISADecodeInfo;

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
