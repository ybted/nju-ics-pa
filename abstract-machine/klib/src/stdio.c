#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *start = out;
  
  for (; *fmt != '\0'; ++fmt) {
    if (*fmt != '%') {
      *out = *fmt;
      ++out;
    } else {
      switch (*(++fmt)) {
      case '%': *out = *fmt; ++out; break;
      case 'd': out += itoa(va_arg(ap, int), out, 10); break;
      case 's':
        char *s = va_arg(ap, char*);
        strcpy(out, s);
        out += strlen(out);
        break;
      }
    }
  }
  
  *out = '\0';
  return out - start;
}


int sprintf(char *out, const char *fmt, ...) {
  va_list pArgs;
  va_start(pArgs, fmt);
  char *start = out;
  
  for (; *fmt != '\0'; ++fmt) {
    if (*fmt != '%') {
      *out = *fmt;
      ++out;
    } else {
      switch (*(++fmt)) {
      case '%': *out = *fmt; ++out; break;
      case 'd': out += itoa(va_arg(pArgs, int), out, 10); break;
      case 's':
        char *s = va_arg(pArgs, char*);
        strcpy(out, s);
        out += strlen(out);
        break;
      }
    }
  }
  *out = '\0';
  va_end(pArgs);

  return out - start;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
