#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, events_read, invalid_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}


int fs_open(const char *pathname, int flags, int mode)
{
  int i = 0;
  int len = sizeof(file_table) / sizeof(file_table[0]);
  for (; i < len; i ++)
  {
    if (strcmp(pathname, file_table[i].name) == 0) 
      break;
  }
  if (i == len)
    assert(0);
  file_table[i].open_offset = 0;
  return i;
}

size_t fs_read(int fd, void *buf, size_t len) 
{
  Finfo file = file_table[fd];
  // printf("filename: %s\n", file.name);
  // assert(file.open_offset + len <=  file.size);
  int read_size;
  if (file.read) {
    read_size = file.read(buf, file.disk_offset + file.open_offset, len);
  } else {
    read_size = ramdisk_read(buf, file.disk_offset + file.open_offset, len);
  }
  file_table[fd].open_offset += read_size;
  return read_size;
}

size_t fs_lseek(int fd, size_t offset, int whence)
{
  if (whence == SEEK_SET)
  {
    file_table[fd].open_offset = offset;
  } else if (whence == SEEK_CUR)
  {
    file_table[fd].open_offset += offset;
  } else if (whence == SEEK_END)
  {
    file_table[fd].open_offset = file_table[fd].size + offset;
  }
  return file_table[fd].open_offset;
}

size_t fs_write(int fd, const void *buf, size_t len)
{
  Finfo file = file_table[fd];
  // assert(file.open_offset + len <= file.size);
  int write_size;
  if (file.write) {
    write_size = file.write(buf, file.disk_offset + file.open_offset, len);
  } else {
    write_size = ramdisk_write(buf, file.disk_offset + file.open_offset, len);
  }
  file_table[fd].open_offset += write_size;
  return write_size;
}


int fs_close(int fd)
{
  return 0;
}