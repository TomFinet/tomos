#pragma once

#define SYMBOL_DEFINE(symbol_name, type)                                       \
	extern void *_##symbol_name;                                           \
	static type symbol_name

#define SYMBOL_READ(name, type) ((type) & name)
