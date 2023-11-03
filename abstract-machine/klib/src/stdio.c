#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
  
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int to_string(char* out, int num) {
  int len = 0;
  if (num < 0) {
    *out++ = '-';
    num *= -1;
    len ++;
  }
  int tmp = 0;
  int tmpnum = num;
  while (tmpnum) {
    tmpnum /= 10;
    tmp ++;
  }
  for (int i = tmp; i > 0; i --) {
    int base = 1;
    for (int j = 0; j < i; j ++)
      base *= 10;
    *out++ = num % base + '0';
    num -= (num % base) * base;
  }
  len += tmp;
  return len;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len = 0;
  while (*fmt) {
    if (*fmt == '%') {
      fmt ++;
      if (*fmt == 'd') {
        int num = va_arg(args, int);
        int inc = to_string(out, num);
        len += inc;
        out += inc;
      } else if (*fmt == 's') {
        char* str = va_arg(args, char*);
        int tmp = strlen(str);
        len += tmp;
        for (int i = 0; i < tmp; i ++){
          *out ++ = str[i];
        }
      }
      fmt ++;
    } else {
      *out++ = *fmt++;
      len ++;
    }
  }
  *out = '\0';
  va_end(args);
  return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
