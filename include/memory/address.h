#pragma once

#include <memory/memory_def.h>

#include <stdint.h>

/* a physical address */
typedef uint32_t pa_t;

/* a virtual address */
typedef uint32_t va_t;

static inline pa_t __pa(va_t va)
{
	return va - KERNEL_VA_BASE;
}

static inline va_t __va(pa_t pa)
{
	return pa + KERNEL_VA_BASE;
}
