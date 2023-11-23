#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tc;
  gettimeofday(&tc, NULL);
  return tc.tv_usec / 1000 + tc.tv_sec * 1000;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", O_RDONLY);
  // printf("fd: %d\n", fd);
  return read(fd, buf, len);
  
}
static int canvas_w, canvas_h, canvas_x = 0, canvas_y = 0;
void NDL_OpenCanvas(int *w, int *h) {
  if (*w == 0){
    *w = screen_w;
  }else if(*w > screen_w){
    assert(1 == 2);
  }
  if (*h == 0){
    *h = screen_h;
  }else if(*h > screen_h){
    assert(1 == 2);
  }
  canvas_w = *w;
  canvas_h = *h;
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }

  
  
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  int fd = open("/proc/dispinfo", O_RDONLY);
  char buffer[100];
  int size = read(fd, buffer, 100);
  printf("buffer: %s\n", buffer);
  sscanf(buffer, "WIDTH:%dHEIGHT:%d", &screen_w, &screen_w);
  printf("w: %d h: %d\n", screen_w, screen_h);
  return 0;
}

void NDL_Quit() {
}
