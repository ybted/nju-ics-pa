#include <NDL.h>

int SDL_Init(uint32_t flags) {
  assert(1 == 2);
  return NDL_Init(flags);
}

void SDL_Quit() {
  assert(1 == 2);
  NDL_Quit();
}

char *SDL_GetError() {
  assert(1 == 2);
  return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char* fmt, ...) {
  assert(1 == 2);
  return -1;
}

int SDL_ShowCursor(int toggle) {
  assert(1 == 2);
  return 0;
}

void SDL_WM_SetCaption(const char *title, const char *icon) {
  assert(1 == 2);
}
