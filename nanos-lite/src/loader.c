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
  Elf_Phdr ph;
  ramdisk_read(&elf, SEEK_SET, sizeof(Elf_Ehdr));
  printf("elf.e_phoff: %d\n", elf.e_phoff);
  ramdisk_read(&ph, SEEK_SET + elf.e_phoff , sizeof(Elf_Phdr));
  printf("%s\n", elf.e_ident);
  assert(elf.e_ident[0] == 0x7f &&
         elf.e_ident[1] == 0x45 &&
         elf.e_ident[2] == 0x4c &&
         elf.e_ident[3] == 0x46);
  printf("ph.p_type: %d\n", ph.p_type);
  assert(ph.p_flags == (PF_X | PF_W | PF_R));
  // ramdisk_read(pf + VirtualAddress, start_of_load,Memsize);
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {

  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", (void *)entry);
  ((void(*)())entry) ();
}

