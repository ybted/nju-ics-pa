#include <proc.h>
#include <elf.h>
#include <stdio.h>
#include <fs.h>
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  Elf_Ehdr elf;
  fs_read(fd, &elf, sizeof(elf));
  
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
  fs_lseek(fd, offset, SEEK_SET);
  for (int i = 0; i < num; i ++) {
    fs_lseek(fd, offset+i*size, SEEK_SET);
    fs_read(fd, &ph[i], size);
  }
  for (int i = 0; i < num; i ++) {
    if(ph[i].p_type == PT_LOAD) {
      fs_lseek(fd, ph[i].p_offset, SEEK_SET);
      fs_read(fd, (void*)(ph[i].p_vaddr), ph[i].p_memsz);
      memset((void*)(ph[i].p_vaddr+ph[i].p_filesz), 0, ph[i].p_memsz - ph[i].p_filesz);
    }
  }
  
  assert(elf.e_ident[0] == 0x7f &&
         elf.e_ident[1] == 0x45 &&
         elf.e_ident[2] == 0x4c &&
         elf.e_ident[3] == 0x46 );
  return elf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", (void *)entry);
  ((void(*)())entry) ();
}

void context_uload(PCB* pro, char* name)
{
  Area ustack;
  ustack.end = heap.end;
  ustack.start = ustack.end - STACK_SIZE;
  uintptr_t entry = loader(pro, name);
  Context* cp = ucontext(NULL, ustack, (void *)entry);
  pro->cp = cp;
  cp->GPRx = (int)cp;
}


void context_kload(PCB* pro, void (*func)(void *), void * arg) {
  Area kstack;
  kstack.end = &pro->stack[STACK_SIZE - 1];
  kstack.start = &pro->stack[0];
  Context* cp = kcontext(kstack, func, arg);
  pro->cp = cp;
}
