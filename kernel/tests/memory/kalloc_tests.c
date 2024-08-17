#include <memory/kslab.h>
#include <memory/frame.h>
#include <memory/paging.h>
#include <memory/kalloc.h>
#include <tests/ktest.h>

static void suite_init(void)
{
	frame_init();
	page_init();
	kcache_init();
}

static void suite_exit(void)
{
}

static void test_kmalloc()
{
	void *mem = kmalloc(4);
	struct kslab_t *slab = page_descriptor(mem)->slab;
	struct kcache_t *cache =
		list_entry(slab->list.next, struct kcache_t, slab_partial);
	ASSERT(cache->objsize == 4);
}

static struct ktest_case_t kalloc_cases[] = {
	KTEST(test_kmalloc),
};

static struct ktest_suite_t kalloc_suite = {
	.name = "kalloc_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = kalloc_cases,
	.num_cases = KTEST_NUM_CASES(kalloc_cases),
};

KTEST_SUITE(kalloc_suite);
