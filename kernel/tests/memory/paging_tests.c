#include <interrupts/isr.h>
#include <memory/frame.h>
#include <memory/paging.h>
#include <tests/ktest.h>

static void suite_init(void)
{
	frame_init();
	page_init();
}

static void suite_exit(void)
{
}

/**
 * Startup.s sets up the page directory and 3 page tables.
 * One is an identity mapped page table, i.e. virtual address is physical address.
 * The other two correspond to the upper quarter kernel, 2048 pages are mapped
 * starting from 0xc0000000, thus ending at 0xc0400000
 * At startup, we only want to allocate enough for the kernel text and data.
 * The rest can be done on demand.
 */
void test_paging_alloc_and_free(void)
{
	va_t page = (va_t)kpage_alloc();
	ASSERT(page >= KERNEL_VA_BASE);
	ASSERT(page_free(page) == PAGE_FREE_SUCCESS);
}

void test_paging_double_free(void)
{	
	va_t page = (va_t)kpage_alloc();
	ASSERT(page >= KERNEL_VA_BASE);
	ASSERT(page_free(page) == PAGE_FREE_SUCCESS);
	ASSERT(page_free(page) == PAGE_ALREADY_FREE);
}

void test_paging_bulk_alloc_and_free(void)
{
	#define n 10
	va_t pages[n];
	pages[0] = (va_t)kpage_alloc();
	ASSERT(pages[0] >= KERNEL_VA_BASE);
	for (int i = 1; i < n; i++) {
		pages[i] = (va_t)kpage_alloc();	
		ASSERT(pages[i] == pages[i - 1] + PAGE_NBYTES);
	}

	for (int i = 0; i < n; i++) {
		ASSERT(page_free(pages[i]) == PAGE_FREE_SUCCESS);
	}
}

static bool entered_isr = false;
static int* null_ptr = NULL;
static void* resume_at = NULL;
static void test_paging_nullptr_page_fault_handler(struct isr_frame* frame);

__attribute__((optimize("O0"))) void test_paging_nullptr_page_fault(void)
{
	isr_register(14, test_paging_nullptr_page_fault_handler);
	entered_isr = false;
	resume_at = &&resume;

	*null_ptr = 42;
	goto resume;
resume:
	ASSERT(entered_isr);
}

static void test_paging_nullptr_page_fault_handler(struct isr_frame* frame)
{
	entered_isr = true;
	frame->eip = (uint32_t)resume_at;
}

static struct ktest_case_t paging_cases[] = {
	KTEST(test_paging_alloc_and_free),
	KTEST(test_paging_double_free),
	KTEST(test_paging_bulk_alloc_and_free),
	KTEST(test_paging_nullptr_page_fault)
};

static struct ktest_suite_t paging_suite = {
	.name = "paging_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = paging_cases,
	.num_cases = KTEST_NUM_CASES(paging_cases),
};

KTEST_SUITE(paging_suite);
