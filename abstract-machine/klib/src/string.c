#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  assert(s);
  for (; s[len] != '\0'; len ++) ;
  return len;
}

char *strcpy(char *dst, const char *src) {
  assert(dst != NULL && src != NULL);
  char* addr = dst;
  while ((*dst++ = *src++) != '\0') ;
  return addr;
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  char* p = dst + strlen(dst);
  while (*src) {
    *p ++ = *src ++;
  }
  *p = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) 
  {
    s1 ++;
    s2 ++;
  }
  return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  
  unsigned char *xs =  s;
  while (n--) 
    *xs++ = c;

  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  assert(out && in && n >= 0);
  char *dst = (char*)out;
  char *src = (char*)in;

  while (n-- > 0) 
    *dst++ = *src++;
  return dst;

}

int memcmp(const void *s1, const void *s2, size_t n) {
  assert(s1 && s2 && n >= 0);
  const char* p1 = (char*)s1;
  const char* p2 = (char*)s2;

  while (n -- && *p1 && *p2 && (*p1 == *p2)) {
      p1 ++;
      p2 ++; 
  }
  return n == 0 ? 0 : *(const unsigned char*)p1 - *(const unsigned char*)p2;
}

#endif
