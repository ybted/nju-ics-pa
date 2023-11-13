#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = io_read(AM_INPUT_KEYBRD).keydown;
  kbd->keycode = io_read(AM_INPUT_KEYBRD).keycode;
}
