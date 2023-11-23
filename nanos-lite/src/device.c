#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif
#define _NUM(x) #x 
#define NUM(x) _NUM(x)
#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (int i = 0; i < len; i ++) {
    putch(*(char *)(buf+i));
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);  
  if (ev.keycode)
    printf("%d\n", ev.keycode);
  if (ev.keycode == AM_KEY_NONE) 
    return 0;
  char* tag = ev.keydown == 0 ? "ku" : "kd";
  strcpy(buf, tag);
  strcat(buf, keyname[ev.keycode]);
  return sizeof(keyname[ev.keycode]);
}
  

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T ev = io_read(AM_GPU_CONFIG);
  int width = ev.width;
  int high = ev.height;
  char* w = "WIDTH:";
  char* h = "HEIGHT:";
  printf("w: %s\n",  NUM(width));
  w = strcat(w, NUM(width));
  strcat(h, NUM(high));
  printf("w: %s\n", w);
  width ++;
  high ++;
  strcpy(buf, w);
  strcat(buf, h);
  
  return sizeof(buf);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
