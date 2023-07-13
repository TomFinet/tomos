#include <tests/ktest.h>
#include <memory/paging.h>
#include <memory/mm.h>

static uint32_t kernel_pend;

static void suite_init(void) {
  kernel_pend = (uint32_t) &_kernel_physical_end;
  mm_init();
  page_init();
}

static void suite_exit(void) { }

void test_paging_alloc_and_free(void) {
  ASSERT(page_alloc(0x0) == PAGE_ALLOC_SUCCESS);
  ASSERT(page_free(0x0) == PAGE_FREE_SUCCESS);
}

void test_paging_double_alloc(void) {
  ASSERT(page_alloc(0x0) == PAGE_ALLOC_SUCCESS);
  ASSERT(page_alloc(0x0) == PAGE_IN_USE);
}

void test_paging_double_free(void) {
  ASSERT(page_alloc(0x0) == PAGE_ALLOC_SUCCESS);
  ASSERT(page_free(0x0) == PAGE_FREE_SUCCESS);
  ASSERT(page_free(0x0) == PAGE_ALREADY_FREE);
}

static struct ktest_case_t paging_cases[] = {
  KTEST(test_paging_alloc_and_free),
};

static struct ktest_suite_t paging_suite = {
  .name = "paging_suite",
  .init = suite_init,
  .exit = suite_exit,
  .cases = paging_cases,
  .num_cases = (sizeof(paging_cases) / sizeof(struct ktest_case_t)),
};

KTEST_SUITE(paging_suite);
