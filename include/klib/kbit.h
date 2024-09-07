#pragma once

#define BITSET(b, n)	((b) << (n))
#define BIT(n)		BITSET(1, n)
#define BITGET(x, n)	((x) & BIT(n))
#define BITMASK(n)	(BIT(n) - 1)
