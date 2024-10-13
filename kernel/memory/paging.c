#include <memory/page_table.h>
#include <memory/page_alloc.h>
#include <memory/paging.h>
#include <memory/zone.h>

#include <klib/kbitmap.h>
#include <klib/kstring.h>

#include <ksymbol.h>

#include <stdbool.h>

SYMBOL_DEFINE(kernel_va_end, va_t);
SYMBOL_PA_DEFINE(tmp_pg_table, pte_t *);

// TODO: bitmap or free list to manage 2^20 pages?
BITMAP(page_free_map, PAGE_COUNT);

static bool init_done = false;
void page_init(void)
{
	if (init_done) {
		return;
	}

	// unmap the first pde, startup uses it for identity mapping
	write_pde(0, 0);

	pg_idx_t kstart_page_idx = PAGE_IDX(KERNEL_VA_BASE);
	pg_idx_t kend_page_idx = PAGE_IDX(kernel_va_end);
	fr_idx_t kstart_fr_idx = ZONE_LINEAR_FRAME_IDX(kstart_page_idx);
	fr_idx_t kend_fr_idx = ZONE_LINEAR_FRAME_IDX(kend_page_idx);

	// mark pages in linear zone occupied by bootloader + kernel as used
	zone_t *zone_linear = get_zone_linear();
	zone_mark_pages_used(zone_linear, kstart_page_idx, kend_page_idx);

	// mark the corresponding page frames as used
	for (fr_idx_t i = kstart_fr_idx; i < kend_fr_idx; i++) {
		alloc_frame(i);
	}

	// reset the identity page table to be used for temporary mappings
	reset_tmp_pg_table();
	// add the tmp page table to the page directory
	pg_idx_t tmp_table_pg_idx = PAGE_IDX((va_t)tmp_pg_table);
	fr_idx_t tmp_table_fr_idx = ZONE_LINEAR_FRAME_IDX(tmp_table_pg_idx);
	pa_t tmp_table_fr_pa = tmp_table_fr_idx << PAGE_ORDER;
	pde_t tmp_table_pde = PAGE_STD(tmp_table_fr_pa);
	write_pde(5, tmp_table_pde); // lets just use 5 as the pde idx corresponding to tmp table

	init_done = true;
}
