#include <NDL.h>
#include <SDL.h>
#include <assert.h>
int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) {
  assert(1 == 2);
  return 0;
}

void SDL_CloseAudio() {
  assert(1 == 2);
}

void SDL_PauseAudio(int pause_on) {
  assert(1 == 2);
}

void SDL_MixAudio(uint8_t *dst, uint8_t *src, uint32_t len, int volume) {
  assert(1 == 2);
}

SDL_AudioSpec *SDL_LoadWAV(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len) {
  assert(1 == 2);
  return NULL;
}

void SDL_FreeWAV(uint8_t *audio_buf) {
  assert(1 == 2);
}

void SDL_LockAudio() {
  assert(1 == 2);
}

void SDL_UnlockAudio() {
  assert(1 == 2);
}
