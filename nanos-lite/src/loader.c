#include <proc.h>
#include <elf.h>
#include <stdio.h>
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf;
  // Elf_Phdr ph;
  ramdisk_read(&elf, 0, sizeof(Elf_Ehdr) * 10);
  // printf("elf.e_type: %d\n", (int)elf.e_type);
  // printf("elf.e_machine: %d\n", (int)elf.e_machine);
  // printf("elf.e_version: %d\n", (int)elf.e_version);
  // printf("elf.e_entry: %d\n", (int)elf.e_entry);
  // printf("elf.e_phoff: %d\n", (int)elf.e_phoff);
  // printf("elf.e_shoff: %d\n", (int)elf.e_shoff);
  // printf("elf.e_flags: %d\n", (int)elf.e_flags);
  // printf("elf.e_ehsize: %d\n", (int)elf.e_ehsize);
  // printf("elf.e_phentize: %d\n", (int)elf.e_phentsize);
  // printf("elf.e_phnum: %d\n", (int)elf.e_phnum);
  // printf("elf.e_shentsize: %d\n", (int)elf.e_shentsize);
  // printf("elf.e_shnum: %d\n", (int)elf.e_shnum);
  // printf("elf.e_shstrndx: %d\n", (int)elf.e_shstrndx);
  // assert(elf.e_phoff != 0);
  int num = elf.e_phnum;
  int offset = elf.e_phoff;
  int size = elf.e_phentsize;
  Elf_Phdr ph[num];
  for (int i = 0; i < num; i ++) {
    ramdisk_read(&ph[i], offset + i * size, size);
  }
  
  
  assert(elf.e_ident[0] == 0x7f &&
         elf.e_ident[1] == 0x45 &&
         elf.e_ident[2] == 0x4c &&
         elf.e_ident[3] == 0x46 );
  // printf("ph.p_type: %d\n", ph.p_type);
  assert(ph[0].p_flags == PF_R);
  // ramdisk_read(pf + VirtualAddress, start_of_load,Memsize);
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {

  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", (void *)entry);
  ((void(*)())entry) ();
}

