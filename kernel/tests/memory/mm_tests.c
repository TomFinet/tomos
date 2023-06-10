#include <tests/ktest.h>
#include <memory/mm.h>

void suite_init(void) {
    mm_init();
}

void suite_exit(void) { }

void test_mm_alloc_frame(void) {
    uint32_t frame = alloc_frame();
    ASSERT(frame); // address was returned
    ASSERT(frame > (uint32_t) &_kernel_physical_end); // in the correct zone
}

static struct ktest_case_t mm_cases[] = {
    KTEST(test_mm_alloc_frame)
};

static struct ktest_suite_t mm_suite = {
    .name = "mm_suite",
    .init = suite_init,
    .exit = suite_exit,
    .cases = mm_cases
};

KTEST_SUITE(mm_suite);
