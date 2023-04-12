#include <klib/kstring.h>

/// TODO: Memset is buggy
void* memset(void* ptr, int c, size_t n) {
	void* optr = ptr;
	asm volatile( "rep stosb" : "=D"(ptr), "=c"(n) : "0"(ptr), "1"(n), "a"(c) : "memory");
	return optr;
}

void bzero(void* ptr, size_t n) {
	memset(ptr, 0, n);
}