#include <elf.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>
#include "../../include/utils.h"

funcInfo elf_func[1024];


static size_t readSymTab(Elf32_Sym **elf32Sym, Elf32_Shdr *elf32Sec, FILE *fp) {
  rewind(fp);

  // 获得节在文件中相对于文件头位置的偏移,并将文件指针移到相应位置
  size_t sh_offset = elf32Sec->sh_offset;   
  fseek(fp, sh_offset, SEEK_SET);

  size_t sh_size = elf32Sec->sh_size;   // 节大小
  size_t sh_entsize = elf32Sec->sh_entsize;   // 有些节的表项是固定大小，比如符号表，这里表示符号表条目的大小
  if (sh_size % sh_entsize != 0) {
    // 说明没有整数个节
    return 0;
  }

  size_t entries = sh_size / sh_entsize;     // 条目个数

  *elf32Sym = (Elf32_Sym*) malloc (sh_size);
  if (*elf32Sym == NULL) {
    return 0;
  }

  size_t result = fread(*elf32Sym, sh_entsize, entries, fp);
  rewind(fp);
  if (result != entries) {
    // free(elf32Sym);
    return 0;
  }
  return sh_size / sh_entsize;
}

static int readStrTab(const Elf32_Sym *elf32Sym, size_t n, Elf32_Shdr *elf32Sec, FILE *fp) {
  rewind(fp);

  size_t sh_offset = elf32Sec->sh_offset;   
  fseek(fp, sh_offset, SEEK_SET);

  size_t index = 0;

  for (int i = 0; i < n; ++i) {
    if (ELF32_ST_TYPE(elf32Sym[i].st_info) != STT_FUNC) {
      continue;
    }
    elf_func[index].start = elf32Sym[i].st_value;
    elf_func[index].size = elf32Sym[i].st_size;
    size_t offset = elf32Sym[i].st_name;
    fseek(fp, sh_offset + offset, SEEK_SET);
    int temp = fscanf(fp, "%63s", elf_func[index].func_name);
    temp ++;
    printf("offset = 0x%x, file offset = 0x%lx\n", elf32Sym[i].st_name, elf32Sym[i].st_name + sh_offset);
    index++;
  }
  rewind(fp);
  elf_func[index].func_name[0] = '\0';

  return 0;
}


static int readSection(Elf32_Ehdr *elfHead32, FILE *fp)
{
  size_t result;
  size_t secNum = elfHead32->e_shnum;
  int secindex = elfHead32->e_shoff;

  printf("SHT_STRTAB = 0x%x\n", elfHead32->e_shoff+ elfHead32->e_shentsize * elfHead32->e_shstrndx);
  Elf32_Shdr shstrtab;
  fseek(fp, elfHead32->e_shoff+ elfHead32->e_shentsize * elfHead32->e_shstrndx, SEEK_SET);
  int n = fread(&shstrtab, sizeof(Elf32_Shdr), 1, fp);
  n ++;
  //go to the entry of section
  /****************
   * secindex 保存头结构体中的e_shoff，该成员表示“Section header table file offset”，
   * 既，section相对于ELF文件的偏移，程序随后seek了这个偏移量（fseek(pFile,secindex,SEEK_SET);）
   * 此时pFIle所读取的位置就是实际的Setion存储的位置了。
   * ****************************/
  fseek(fp,secindex,SEEK_SET);
  
  Elf32_Shdr elf32Sec;

  Elf32_Sym *elf32Sym = NULL;
  size_t sym_n = 0;
  
  for(int i = 0;i < secNum;i++)
  {
    result = fread(&elf32Sec,1,sizeof(Elf32_Shdr), fp); 
    if(result != sizeof(Elf32_Shdr))
    {
      assert(0);
      // panic("read file failed\n");
      return -1;
    }
    // 暂时不读取动态符号表
    if (elf32Sec.sh_type == SHT_SYMTAB) {
      // SHT_SYMTAB SHT_DYNSYM 这两类节都含有符号表,只能各包含一个这两种节。SHT_SYMTAB 提供的符号用于在创建ELF文件的时候编辑连接,在运行期间也有可能会用于动态连接
      if(!(sym_n = readSymTab(&elf32Sym, &elf32Sec, fp))) {
        // panic("read symbol table failure.");
        assert(0);
      }
      break;
    }
  }
  rewind(fp);
  fseek(fp,secindex,SEEK_SET);
  for (int i = 0; i < secNum; ++i) {
    fseek(fp,secindex + i * sizeof(Elf32_Shdr),SEEK_SET);
    result = fread(&elf32Sec,1,sizeof(Elf32_Shdr), fp); 
    if(result != sizeof(Elf32_Shdr))
    {
      // panic("read file failed\n");
      assert(0);
      return -1;
    }
    /****************
     * 有可能是.dynstr  .strtab   .shstrtab
     * 首先.shstrtab很好识别，只要看当前读取节头在节头表中的序号是否和包存在头文件中的一致，一致，则说明是.shstrtab
     * 其次，.dynstr .strtab的区分，可根据sh_name找到在.shstrtab节中的位置，读取其第二个字符，判断是否d和s 
    */
    if (elf32Sec.sh_type == SHT_STRTAB) {
      printf("offset = 0x%x\n", elf32Sec.sh_offset);
      if (i == elfHead32->e_shstrndx) {
        // 为shstrtab
        continue;
      }
      rewind(fp);
      fseek(fp, shstrtab.sh_offset + elf32Sec.sh_name + 1, SEEK_SET);
      char c;
      int m = fread(&c, 1, 1, fp);
      m ++;
      if (c == 'd') {
        continue;
      }
      if(readStrTab(elf32Sym, sym_n, &elf32Sec, fp)) {
        // panic("read string table failure. ");
        assert(0);
      }
      // break;
    }
  }

  rewind(fp);
  if (elf32Sym != NULL) {
    free(elf32Sym);
  }
  return 0;  
}


static Elf32_Ehdr read_elf_head(FILE *fp) {
  char headbuf[EI_NIDENT] = {0};
  rewind (fp);
  int n = fread(headbuf,1,EI_NIDENT, fp);
  n ++;
  //judge if this is a elf file
  if(
    headbuf[0] != 0x7f && headbuf[1] != 0x45 &&
    headbuf[2] != 0x4c && headbuf[3] != 0x46
    )    
  {
    // panic("not a elf file!\n");
        assert(0);
  }
  rewind(fp);
  Elf32_Ehdr elfHead32;
  int q = fread (&elfHead32 , 1, sizeof(Elf32_Ehdr), fp);
  q ++;
  rewind(fp);
  return elfHead32;
}


static void parse_elf(const char *elf_file) {
  if (elf_file ==NULL || strlen(elf_file) == 0) {
    return;
  }
  FILE *fp = fopen(elf_file, "rb");
   long lSize;
  // obtain file size:
  fseek (fp, 0 , SEEK_END);
  lSize = ftell (fp);
  rewind (fp);
  if(lSize < sizeof(Elf32_Ehdr) || lSize < sizeof(Elf32_Ehdr) )
  {
    // panic("not a elf file!\n");
        assert(0);
    return ;
  }

  Elf32_Ehdr elfHead32 = read_elf_head(fp);

  readSection(&elfHead32, fp);

  return ;
}

void init_elf(const char* elf_file)
{
  parse_elf(elf_file);
  printf("Num:       Value       Size Name\n");
  for (int i = 0; i < 1024; ++i) {
    if(elf_func[i].func_name[0] == '\0') {
      break;
    }
    printf("%3d: %016lx %5ld %s\n", i, elf_func[i].start, elf_func[i].size, elf_func[i].func_name);
  }
}


// int main(int argc, char** argv)
// {
//   parse_elf(argv[1]);
//   printf("Num:       Value       Size Name\n");
//   for (int i = 0; i < 1024; ++i) {
//     if(elf_func[i].func_name[0] == '\0') {
//       break;
//     }
//     printf("%3d: %016x %5ld %s\n", i, elf_func[i].start, elf_func[i].size, elf_func[i].func_name);
//   }
// }