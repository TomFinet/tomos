#include <memory/kheap.h>
#include <memory/kslab.h>
#include <memory/mm.h>
#include <memory/paging.h>
#include <memory/vmm.h>
#include <tests/ktest.h>

static void suite_init(void)
{
	mm_init();
	page_init();
	kheap_init();
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

static struct ktest_case_t vmm_cases[] = {
	KTEST(test_kmalloc),
};

static struct ktest_suite_t vmm_suite = {
	.name = "vmm_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = vmm_cases,
	.num_cases = (sizeof(vmm_cases) / sizeof(struct ktest_case_t)),
};

KTEST_SUITE(vmm_suite);
