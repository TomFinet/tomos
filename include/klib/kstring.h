#pragma once

#include <stddef.h>
#include <stdint.h>

void *memset(void *ptr, int c, size_t n);
void bzero(void *ptr, size_t n);
void memcopy(char *dest, char *src, size_t n);