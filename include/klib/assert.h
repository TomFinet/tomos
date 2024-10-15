#pragma once

#include <klib/kstdio.h>
#include <panic.h>

#define assert(x) ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__), 0)))

void __assert_fail(const char *expr, const char *file, int line, const char *func);
