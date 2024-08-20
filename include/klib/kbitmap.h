#pragma once

#include <stdint.h>
#include <stdbool.h>

#define NOT_FOUND -1
#define ALL_SET 0xffffffff

#define BITS_IN_BLK 32

#define BITMAP_BLKS(n) ((n + BITS_IN_BLK - 1) / BITS_IN_BLK)
#define BITMAP(bitmap, n) uint32_t bitmap[BITMAP_BLKS(n)]

#define BITMAP_BLK(n) ((n) / BITS_IN_BLK)
#define BITMAP_POS(n) ((n) % BITS_IN_BLK)

static inline bool bitmap_read(uint32_t* bitmap, int n)
{
	return bitmap[BITMAP_BLK(n)] & (1 << BITMAP_POS(n));
}

static inline void bitmap_set(uint32_t* bitmap, int n)
{
	bitmap[BITMAP_BLK(n)] = bitmap[BITMAP_BLK(n)] | (1 << BITMAP_POS(n));
}

static inline void bitmap_clear(uint32_t* bitmap, int n)
{
	bitmap[BITMAP_BLK(n)] = bitmap[BITMAP_BLK(n)] & ~(1 << BITMAP_POS(n));
}

static inline int bitmap_first_clear(uint32_t* bitmap, int bits_in_bitmap)
{
	int i = 0;
	int size = BITMAP_BLKS(bits_in_bitmap);
	while (i < size && bitmap[i] == ALL_SET) {
		i++;
	}

	if (i == size) {
		return NOT_FOUND;
	}

	int idx = __builtin_ctz(~bitmap[i]);
	return idx + BITS_IN_BLK * i;
}
