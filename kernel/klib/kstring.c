#include <klib/kstring.h>

void* memset(void* ptr, int c, size_t n) {
	void* optr = ptr;
	asm volatile( "rep stosb" : "=D"(ptr), "=c"(n) : "0"(ptr), "1"(n), "a"(c) : "memory");
	return optr;
}

void bzero(void* ptr, size_t n) {
	memset(ptr, 0, n);
}

void memcopy(char* dest, char* src, size_t n) {
  for(size_t i = 0; i < n; i++) {
    *(dest + i) = *(src + i);
  }
}