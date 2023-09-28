#pragma once

#include <klib/kstring.h>
#include <serial.h>
#include <stdarg.h>

void printk(const char *str);
void fprintk(const char *str, ...);
void vprintk(const char *str, va_list list);

char *itoa(uint32_t n, char *str, uint8_t base);