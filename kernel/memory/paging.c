#include <memory/paging.h>

#include <klib/kbitmap.h>
#include <klib/kstring.h>

#include <ksymbol.h>

SYMBOL_DEFINE(page_dir, pde_t *);
SYMBOL_DEFINE(startup_kernel_mapped_pages, int);

// TODO: bitmap or free list to manage 2^20 pages?
BITMAP(page_free_map, PAGE_COUNT);

static inline void page_zero(pte_t *page_table)
{
	memset((void *)page_table, 0, PTE_COUNT * sizeof(pte_t));
}

static inline void page_tlb_invalid(va_t va)
{
	asm volatile("invlpg %0" : : "m"(va) : "memory");
}

static bool init_done = false;
void page_init(void)
{
	if (init_done) {
		return;
	}

	page_dir = (pde_t *)__va(SYMBOL_READ(_page_dir, pa_t));
	startup_kernel_mapped_pages =
		SYMBOL_READ(_startup_kernel_mapped_pages, int);

	int i = 0;
	for (; i < BITMAP_BLK(startup_kernel_mapped_pages); i++) {
		page_free_map[BITMAP_BLKS(KERNEL_PTE_BASE) + i] = ALL_SET;
	}

	int part_blk_idx = BITMAP_POS(startup_kernel_mapped_pages);
	if (part_blk_idx) {
		page_free_map[BITMAP_BLKS(KERNEL_PTE_BASE) + i] =
			(1 << part_blk_idx) - 1;
	}
	init_done = true;
}

void *page_alloc(void)
{
	int bitmap_offset = BITMAP_BLKS(KERNEL_PTE_BASE);
	int start_page_num = KERNEL_PTE_BASE;

	int res = bitmap_first_clear(page_free_map + bitmap_offset,
				     PAGE_COUNT - start_page_num + 1);
	if (res == NOT_FOUND) {
		return NULL;
	}

	int free_page_num = start_page_num + res;
	va_t free_page_va = free_page_num << PAGE_ORDER;
	int pde_idx = PDE_IDX(free_page_va);
	int pte_idx = PTE_IDX(free_page_va);

	if (!IS_PRESENT(page_dir[pde_idx])) {
		pa_t frame = frame_alloc();
		page_zero((pte_t *)__va(frame));
		page_dir[pde_idx] = PAGE_STD(frame);
	}

	pde_t pde = page_dir[pde_idx];
	pte_t *page_table = (pte_t *)__va(PAGE_PA(pde));
	if (IS_PRESENT(page_table[pte_idx])) {
		return NULL;
	}

	pa_t frame = frame_alloc();
	page_table[pte_idx] = PAGE_STD(frame);
	page_tlb_invalid(free_page_va);
	bitmap_set(page_free_map, free_page_num);

	return (void *)free_page_va;
}

int page_free(va_t vbase)
{
	int pde_idx = PDE_IDX(vbase);
	int pte_idx = PTE_IDX(vbase);
	pde_t pde = page_dir[pde_idx];

	if (!IS_PRESENT(pde)) {
		return PAGE_ALREADY_FREE;
	}

	pte_t *page_table = (pte_t *)__va(PAGE_PA(pde));
	pte_t pte = page_table[pte_idx];

	if (!IS_PRESENT(pte)) {
		return PAGE_ALREADY_FREE;
	}

	page_table[pte_idx] &= ~PAGE_PRESENT(1);
	page_tlb_invalid(vbase);
	bitmap_clear(page_free_map, PAGE_IDX(vbase));
	frame_free(PAGE_PA(pte));

	return PAGE_FREE_SUCCESS;
}

struct frame_t *page_descriptor(void *va)
{
	pa_t page_frame_addr = __pa((va_t)va);
	return frame_from_pa(page_frame_addr);
}