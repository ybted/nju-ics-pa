#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>
#include <assert.h>
SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
  assert(1 == 2);
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
  assert(1 == 2);
  return 1;
}

uint32_t SDL_GetTicks() {
  return NDL_GetTicks();
}

void SDL_Delay(uint32_t ms) {
  assert(1 == 2);
}
