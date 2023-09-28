#include <klib/kmath.h>
#include <memory/kheap.h>
#include <memory/mm.h>
#include <memory/paging.h>
#include <tests/ktest.h>

extern va_t kheap_top;

static void suite_init(void)
{
	mm_init();
	page_init();
	kheap_init();
}

static void test_kheap_extend()
{
	va_t prior_kheap_top = kheap_top;
	char *kheap_page = (char *)kheap_extend();
	va_t after_kheap_top = kheap_top;
	ASSERT(prior_kheap_top + PAGE_NBYTES == after_kheap_top);

	/* Check that we can write to the entire page. */
	char magic_value = 0xac;
	for (int i = 0; i < PAGE_NBYTES; i++) {
		*(kheap_page + i) = magic_value;
	}
}

static void suite_exit(void)
{
}

static struct ktest_case_t kheap_cases[] = {
	KTEST(test_kheap_extend),
};

static struct ktest_suite_t kheap_suite = {
	.name = "kheap_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = kheap_cases,
	.num_cases = (sizeof(kheap_cases) / sizeof(struct ktest_case_t)),
};

KTEST_SUITE(kheap_suite);