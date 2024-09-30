#include <memory/frame.h>
#include <memory/paging.h>
#include <memory/page_alloc.h>
#include <tests/ktest.h>
#include <ksymbol.h>

SYMBOL_DEFINE(kernel_pa_end, pa_t);

static void suite_init(void)
{
}

static void suite_exit(void)
{
}

void test_frame_kernel_alloc(void)
{
	page_init();
	int i = 0;
	for (; i * FRAME_NBYTES < kernel_pa_end; i++) {
		ASSERT(!frame_is_free(i));
	}
	ASSERT(frame_is_free(i));
}

void test_frame_alloc_free(void)
{
	uint32_t frame1 = frame_alloc();
	ASSERT(frame1);
	uint32_t aligned_addr = (kernel_pa_end & ~0xfff);
	aligned_addr += aligned_addr < kernel_pa_end ? FRAME_NBYTES : 0;
	assert_eq(frame1, aligned_addr);
	frame_free(frame1);
	uint32_t frame2 = frame_alloc();
	ASSERT(frame1 == frame2);
	frame_free(frame2);
}

void test_frame_alloc_consecutive(void)
{
	uint32_t frame1 = frame_alloc();
	uint32_t frame2 = frame_alloc();
	ASSERT(frame1 + FRAME_NBYTES == frame2);
	frame_free(frame1);
	frame_free(frame2);
}

void test_frame_alloc_between(void)
{
	uint32_t frame1 = frame_alloc();
	uint32_t frame2 = frame_alloc();
	uint32_t frame3 = frame_alloc();
	frame_free(frame2);
	uint32_t between_frame = frame_alloc();
	ASSERT(between_frame == frame2);
	frame_free(frame1);
	frame_free(frame3);
	frame_free(between_frame);
}

static struct ktest_case_t frame_cases[] = {
	KTEST(test_frame_kernel_alloc),
	KTEST(test_frame_alloc_free),
	KTEST(test_frame_alloc_consecutive),
	KTEST(test_frame_alloc_between),
};

static struct ktest_suite_t frame_suite = {
	.name = "frame_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = frame_cases,
	.num_cases = KTEST_NUM_CASES(frame_cases),
};

KTEST_SUITE(frame_suite);
