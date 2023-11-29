#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = (inl(VGACTL_ADDR) >> 16 )& 0xffff;  // TODO: get the correct width
  int h = inb(VGACTL_ADDR) & 0xffff;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = 10;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t screen_wh = inl(VGACTL_ADDR);
  uint32_t h = screen_wh & 0xffff;
  uint32_t w = screen_wh >> 16;
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  // int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  // if (!ctl->sync && (w == 0 || h == 0)) return;
  // uint32_t *pixels = ctl->pixels;
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // int screen_w = (inl(VGACTL_ADDR) >> 16 )& 0xffff;
  // for (int i = y; i < y + h; i ++) {
  //   for (int j = x; j < x + w; j ++) {
  //     fb[j + i * screen_w] = pixels[ j -x + (i - y) * w];
  //   }
  // }
  // if (ctl->sync) {
  //   outl(SYNC_ADDR, 1);
  // }
  int win_weight = io_read(AM_GPU_CONFIG).width;  // TODO: get the correct width

  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *pi = (uint32_t *)(uintptr_t)ctl->pixels;
  // printf("(x:%d, y:%d) w=%d, h=%d", ctl->x, ctl->y, ctl->w, ctl->h);
  for (int i = 0; i < ctl->h; ++i){
    for (int j = 0; j < ctl->w; ++j){
      fb[(ctl->y) * win_weight + i * win_weight + ctl->x + j] = pi[i * (ctl->w) + j];
      //真t娘的奇怪
      //outl((uint32_t)(fb + ctl->y * win_weight + i * win_weight + ctl->x + j), pi[i * (ctl->w) + j]);
    }
  }
  
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}


void __am_gpu_memcpy(AM_GPU_MEMCPY_T *params) {
  uint32_t *src = params->src, *dst = (uint32_t *)(FB_ADDR + params->dest);
  for (int i = 0; i < params->size; i++, src++, dst++){
    *dst = *src;
  }
  char *c_src = (char *)src, *c_dst = (char *)dst;
  for (int i = 0; i < (params->size & 3); i++){
    c_dst[i] = c_src[i];
  }
}