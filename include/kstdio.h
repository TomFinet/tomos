#pragma once

#include <stdarg.h>

#include <serial.h>

void printk(const char* str);
void fprintk(const char* str, ...);
void vprintk(const char* str, va_list list);

char* itoa(uint32_t n, char* str, uint8_t base);