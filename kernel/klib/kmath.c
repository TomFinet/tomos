#include <klib/kmath.h>

static uint32_t next = 1;

uint32_t rand(void)
{
	next = next * 1103515245 + 12345;
	return (next >> 16) % 4096;
}

unsigned int kceil(int a, int b)
{
	return (a + b - 1) / b;
}