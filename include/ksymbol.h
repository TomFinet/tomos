#pragma once

#define SYMBOL_DEFINE(symbol, type)                                       \
	extern void *_##symbol;                                           \
	static type symbol = ((type)&_##symbol)

#define SYMBOL_READ(symbol, type) ((type)&symbol)
