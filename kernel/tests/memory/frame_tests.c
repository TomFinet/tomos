#include <memory/frame.h>
#include <memory/paging.h>
#include <tests/ktest.h>
#include <ksymbol.h>

extern pa_t _kernel_physical_start;
extern pa_t _kernel_physical_end;
static pa_t kernel_end;

static void suite_init(void)
{
	kernel_end = SYMBOL_READ(_kernel_physical_end, pa_t);
	frame_init();
}

static void suite_exit(void)
{
}

void test_frame_kernel_alloc(void)
{
	frame_init();
	int i = 0;
	while (i * FRAME_NBYTES < kernel_end) {
		ASSERT(!is_frame_free(i));
		i++;
	}
	ASSERT(is_frame_free(i));
}

void test_frame_alloc_free(void)
{
	uint32_t frame1 = alloc_frame();
	ASSERT(frame1);
	uint32_t aligned_addr = (kernel_end & ~0xfff);
	aligned_addr += aligned_addr < kernel_end ? FRAME_NBYTES : 0;
	assert_eq(frame1, aligned_addr);
	free_frame(frame1);
	uint32_t frame2 = alloc_frame();
	ASSERT(frame1 == frame2);
	free_frame(frame2);
}

void test_frame_alloc_consecutive(void)
{
	uint32_t frame1 = alloc_frame();
	uint32_t frame2 = alloc_frame();
	ASSERT(frame1 + FRAME_NBYTES == frame2);
	free_frame(frame1);
	free_frame(frame2);
}

void test_frame_alloc_between(void)
{
	uint32_t frame1 = alloc_frame();
	uint32_t frame2 = alloc_frame();
	uint32_t frame3 = alloc_frame();
	free_frame(frame2);
	uint32_t between_frame = alloc_frame();
	ASSERT(between_frame == frame2);
	free_frame(frame1);
	free_frame(frame3);
	free_frame(between_frame);
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
