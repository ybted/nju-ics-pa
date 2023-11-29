#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif


#define NAME(key) \
  [AM_KEY_##key] = #key,

#define NUM(x) NUM_temp(x)
#define NUM_temp(x) #x 


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
  if (ev.keycode == AM_KEY_NONE) return 0;
  
  int real_length = 4;
  char *tag = ev.keydown ? "kd " : "ku ";
  if (real_length <= len){
    strcpy(buf, tag);
  }else {
    assert(0);
    return 0;
  }
  real_length += strlen(keyname[ev.keycode]);
  
  if (real_length<= len){
    strcat(buf, keyname[ev.keycode]);
  }else {
    Log("Need %d for %s%s but got %d", strlen(keyname[ev.keycode]), buf, keyname[ev.keycode], len);
    assert(0);
    return 0;
  }
  Log("Got  (kbd): %s (%d) %s\n", keyname[ev.keycode], ev.keycode, ev.keydown ? "DOWN" : "UP");
  
  return real_length;
}
  
int width, high;

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T ev = io_read(AM_GPU_CONFIG);
  width = ev.width;
  high = ev.height;
  char* w = "WIDTH:";
  char* h = "HEIGHT:";
  char wi[10];
  char hi[10];
  sprintf(wi, "%d", width); 
  sprintf(hi, "%d", high);
 
  strcpy(buf, w);
  strcat(buf, wi);
  strcat(buf, h);
  strcat(buf, hi);

  return sizeof(buf);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  io_write(AM_GPU_MEMCPY, offset, (void *)buf, len);
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
