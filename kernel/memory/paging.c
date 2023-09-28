#include <memory/paging.h>

/* virtual address translation of page_dir */
uint32_t *page_dir;

/* request a frame from the physical memory manager
handling errors with a kernel panic */
static pa_t request_frame(void)
{
	pa_t frame = alloc_frame();
	if (!frame) {
		kpanic();
	}
	return frame;
}

/* reads the page dir at the specified index
and returns the page table address
if the page dir entry is unmapped,
allocate a new frame for the page table. */
static va_t pde_table(uint16_t pde_idx)
{
	va_t page_table;
	pde_t pde = page_dir[pde_idx];
	if (IS_PRESENT(pde)) {
		page_table = __va(PAGE_PA(pde));
	} else {
		pa_t table_frame = request_frame();
		page_table = __va(table_frame);
		page_dir[pde_idx] = PAGE_STD(table_frame);
	}
	return page_table;
}

static inline void page_invalidate(va_t va)
{
	asm volatile("invlpg %0" : : "m"(va) : "memory");
}

void page_init(void)
{
	page_dir = (uint32_t *)__va((uint32_t)&_page_dir);
}

int page_alloc(va_t vbase)
{
	if (!IS_ALIGNED(vbase)) {
		return PAGE_INVALID_BASE;
	}

	uint16_t pde_idx = PAGE_DIR_IDX(vbase);
	uint32_t *page_table = (uint32_t *)pde_table(pde_idx);

	uint16_t pte_idx = PAGE_TABLE_IDX(vbase);
	if (IS_PRESENT(page_table[pte_idx])) {
		return PAGE_IN_USE;
	}

	pa_t frame = request_frame();
	page_table[pte_idx] = PAGE_STD(frame);
	return PAGE_ALLOC_SUCCESS;
}

int page_free(va_t vbase)
{
	if (!IS_ALIGNED(vbase)) {
		return PAGE_INVALID_BASE;
	}

	uint16_t pde_idx = PAGE_DIR_IDX(vbase);
	pde_t pde = page_dir[pde_idx];

	if (!IS_PRESENT(pde)) {
		return PAGE_ALREADY_FREE;
	}

	uint32_t *page_table = (uint32_t *)__va(PAGE_PA(pde));
	uint16_t pte_idx = PAGE_TABLE_IDX(vbase);
	pte_t pte = page_table[pte_idx];

	if (!IS_PRESENT(pte)) {
		return PAGE_ALREADY_FREE;
	}

	page_table[pte_idx] &= ~PAGE_PRESENT(1);
	return PAGE_FREE_SUCCESS;
}

pa_t page_table_read(va_t vbase)
{
	if (!IS_ALIGNED(vbase)) {
		return PAGE_INVALID_BASE;
	}
	pde_t pde = page_dir[PAGE_DIR_IDX(vbase)];
	va_t *page_table = (va_t *)__va(PAGE_PA(pde));
	return PAGE_PA(page_table[PAGE_TABLE_IDX(vbase)]);
}

/* given a virtual address, returns its page frame descriptor.
All of kernel virtual memory is mapped linearly to physical memory. */
struct frame_t *page_descriptor(void *va)
{
	// TODO: may not need to read the page table,
	// kernel virtual memory is mapped linearly to physical memory.
	pa_t page_frame_addr = page_table_read(PAGE_PA((va_t)va));
	return pa_to_frame(page_frame_addr);
}