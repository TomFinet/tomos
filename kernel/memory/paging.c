#include <memory/page_table.h>
#include <memory/paging.h>
#include <memory/zone.h>

#include <klib/kbitmap.h>
#include <klib/kstring.h>

#include <ksymbol.h>

#include <stdbool.h>

SYMBOL_DEFINE(kernel_va_end, va_t);

// TODO: bitmap or free list to manage 2^20 pages?
BITMAP(page_free_map, PAGE_COUNT);

static bool init_done = false;
void page_init(void)
{
	if (init_done) {
		return;
	}

	pg_idx_t kstart_page_idx = PAGE_IDX(KERNEL_VA_BASE);
	pg_idx_t kend_page_idx = PAGE_IDX(kernel_va_end);
	fr_idx_t kstart_fr_idx = ZONE_LINEAR_FRAME_IDX(kstart_page_idx);
	fr_idx_t kend_fr_idx = ZONE_LINEAR_FRAME_IDX(kend_page_idx);

	// mark pages in linear zone occupied by bootloader + kernel as used
	// setting the corresponding page table mappings if not already done
	zone_t *zone_linear = get_zone_linear();
	zone_mark_pages_used(zone_linear, kstart_page_idx, kend_page_idx);

	// mark the corresponding page frames as used
	for (fr_idx_t i = kstart_fr_idx; i < kend_fr_idx; i++) {
		alloc_frame(i);
	}

	init_done = true;
}

/*void *page_alloc(void)
{
	int bitmap_offset = BITMAP_BLK(KERNEL_PTE_BASE);
	int res = bitmap_first_clear(page_free_map + bitmap_offset,
				     PAGE_COUNT - KERNEL_PTE_BASE + 1);
	if (res == NOT_FOUND) {
		return NULL;
	}

	int free_page_num = KERNEL_PTE_BASE + res;
	int pde_idx = PDE_IDX(free_page_num);
	int pte_idx = PTE_IDX(free_page_num);

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
	va_t free_page_va = PAGE_VA(free_page_num);
	page_tlb_invalid(free_page_va);
	bitmap_set(page_free_map, free_page_num);

	return (void *)free_page_va;
}

int page_free(va_t vbase)
{
	int page_idx = PAGE_IDX(vbase);
	int pde_idx = PDE_IDX(page_idx);
	int pte_idx = PTE_IDX(page_idx);
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
	bitmap_clear(page_free_map, page_idx);
	frame_free(PAGE_PA(pte));

	return PAGE_FREE_SUCCESS;
}*/
