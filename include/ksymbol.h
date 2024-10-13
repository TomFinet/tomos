#pragma once

#include <memory/memory_def.h>

#define SYMBOL_DEFINE(symbol, type)                                       \
	extern void *_##symbol;                                           \
	static type symbol = ((type)&_##symbol)

#define SYMBOL_PA_DEFINE(symbol, type)	\
	extern void *_##symbol;		\
	static type symbol = ((type)(((pa_t)(&_##symbol)) + KERNEL_VA_BASE))

#define SYMBOL_READ(symbol, type) ((type)&symbol)
