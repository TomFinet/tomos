#pragma once

#include <stdint.h>

#define KERNEL_BASE 0xC0000000 /* 3GB */ // TODO: read from linker script

/* a physical address */
typedef uint32_t pa_t;

/* a virtual address */
typedef uint32_t va_t;

static inline pa_t __pa(va_t va)
{
	return va - KERNEL_BASE;
}

static inline va_t __va(pa_t pa)
{
	return pa + KERNEL_BASE;
}
