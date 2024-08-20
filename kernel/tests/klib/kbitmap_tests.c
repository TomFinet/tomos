#include <klib/kbitmap.h>
#include <klib/kstring.h>
#include <tests/ktest.h>

static void suite_init(void)
{
}

static void suite_exit(void)
{
}

void kbitmap_set_test(void)
{
	BITMAP(map, 150);
	memset(map, 0, 20);

	bitmap_set(map, 0);
	ASSERT((map[0] & ALL_SET) == 0x1);
	bitmap_set(map, 31);
	ASSERT((map[0] & ALL_SET) == 0x80000001);
	bitmap_set(map, 0);
	ASSERT((map[0] & ALL_SET) == 0x80000001);

	bitmap_set(map, 132);
	ASSERT((map[4] & ALL_SET) == 0x00000010);
	bitmap_set(map, 133);
	ASSERT((map[4] & ALL_SET) == 0x00000030);
}

void kbitmap_clear_test(void)
{
	BITMAP(map, 150);
	memset(map, 0, 20);

	bitmap_set(map, 0);
	ASSERT((map[0] & ALL_SET) == 0x1);
	bitmap_set(map, 7);
	ASSERT((map[0] & ALL_SET) == 0x81);
	bitmap_clear(map, 0);
	ASSERT((map[0] & ALL_SET) == 0x80);
	bitmap_clear(map, 0);
	ASSERT((map[0] & ALL_SET) == 0x80);

	ASSERT((map[3] & ALL_SET) == 0x0);
	bitmap_set(map, 96);
	ASSERT((map[3] & ALL_SET) == 0x1);
	bitmap_clear(map, 96);
	ASSERT((map[3] & ALL_SET) == 0x0);
}

void kbitmap_read_test(void)
{
	BITMAP(map, 16 * 8);
	memset(map, 0, 16);

	ASSERT(bitmap_read(map, 10) == false);
	bitmap_set(map, 10);
	ASSERT(bitmap_read(map, 10) == true);
	bitmap_clear(map, 10);
	ASSERT(bitmap_read(map, 10) == false);

	ASSERT(bitmap_read(map, 87) == false);
	bitmap_set(map, 87);
	ASSERT(bitmap_read(map, 87) == true);
}

void kbitmap_first_clear_test(void)
{
	BITMAP(map, 150);
	memset(map, 0, 20);

	ASSERT(bitmap_first_clear(map, 150) == 0);
	map[0] = ALL_SET;
	ASSERT(bitmap_first_clear(map, 150) == 32);
	map[0] &= 0xfffeffff;
	ASSERT(bitmap_first_clear(map, 150) == 16);
	map[0] = ALL_SET;
	map[1] |= 0x0000000f;
	ASSERT(bitmap_first_clear(map, 150) == 36);
	map[1] = ALL_SET;
	map[2] = ALL_SET;
	map[3] = ALL_SET;
	map[4] = ALL_SET;
	ASSERT(bitmap_first_clear(map, 150) == NOT_FOUND);
}

static struct ktest_case_t kbitmap_cases[] = {
	KTEST(kbitmap_set_test),
	KTEST(kbitmap_clear_test),
	KTEST(kbitmap_read_test),
	KTEST(kbitmap_first_clear_test),
};

static struct ktest_suite_t kbitmap_suite = {
	.name = "kbitmap_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = kbitmap_cases,
	.num_cases = KTEST_NUM_CASES(kbitmap_cases),
};

KTEST_SUITE(kbitmap_suite);
