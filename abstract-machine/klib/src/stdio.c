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

int to_hex(char* out, unsigned long num) 
{
  char temp[100];
  assert(num);
  sprintf(temp, "%u", num);
  for (int i = 0; i < strlen(temp); i ++) {
    *out ++ = temp[i];
  }

  return strlen(temp);
}

int vsprintf(char *out, const char *fmt, va_list args) {
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
      } else if (*fmt == 'p') {
        *out++ = '0';
        *out++ = 'x';
        unsigned long num = va_arg(args, unsigned long);
        int inc = to_hex(out, num);
        len += inc;
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
