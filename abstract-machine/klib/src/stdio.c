#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static char printf_buf[1024];

int printf(const char *fmt, ...) {
  va_list args;
  int n;
  va_start(args, fmt);
  n = vsprintf(printf_buf, fmt, args);
  va_end(args);
  for (char* p = printf_buf; *p != '\0'; p ++)
  {
    putch(*p);
  }
  return n;
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
    for (int j = 1; j < i; j ++)
      base *= 10;
    *out++ = num / base + '0';
    num -= (num / base) * base;
  }
  len += tmp;
  return len;
}

int vsprintf(char *out, const char *fmt, va_list args) {
  int len = 0;
  while (*fmt) {
    if (*fmt == '%') {
      fmt ++;
      if (*fmt == 'd' || *fmt == 'p') {
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
  return len;
}


int sprintf(char *out, const char *fmt, ...) {
 va_list args;
 int i;
 va_start(args, fmt);
 i = vsprintf(out, fmt, args);
 va_end(args);
 return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
