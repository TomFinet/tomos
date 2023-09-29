#include <memory/mm.h>
#include <memory/paging.h>
#include <tests/ktest.h>

static pa_t kernel_end;

static void suite_init(void)
{
	kernel_end = (pa_t)&_kernel_physical_end;
	mm_init();
}

static void suite_exit(void)
{
}

void print_frame(uint32_t frame)
{
	fprintk("frame address: %h, kend: %h\n", frame, kernel_end);
}

void test_mm_alloc_free_frame(void)
{
	uint32_t frame1 = alloc_frame();
	ASSERT(frame1);
	uint32_t aligned_addr = (kernel_end & ~0xfff);
	aligned_addr += aligned_addr < kernel_end ? FRAME_NBYTES : 0;
	ASSERT(frame1 == aligned_addr);
	free_frame(frame1);
	uint32_t frame2 = alloc_frame();
	ASSERT(frame1 == frame2);
	free_frame(frame2);
}

void test_mm_alloc_consecutive(void)
{
	uint32_t frame1 = alloc_frame();
	uint32_t frame2 = alloc_frame();
	ASSERT(frame1 + FRAME_NBYTES == frame2);
	free_frame(frame1);
	free_frame(frame2);
}

void test_mm_alloc_between(void)
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

static struct ktest_case_t mm_cases[] = {
	KTEST(test_mm_alloc_free_frame),
	KTEST(test_mm_alloc_consecutive),
	KTEST(test_mm_alloc_between),
};

static struct ktest_suite_t mm_suite = {
	.name = "mm_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = mm_cases,
	.num_cases = KTEST_NUM_CASES(mm_cases),
};

KTEST_SUITE(mm_suite);
