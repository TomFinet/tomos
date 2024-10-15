#include <klib/kbitmap.h>
#include <klib/kstring.h>
#include <klib/assert.h>

#include <memory/zone.h>
#include <memory/pg_tmp.h>
#include <memory/page_table.h>
#include <memory/page_alloc.h>
#include <ksymbol.h>

#include <panic.h>

static pg_idx_t get_free_pg(zone_t *zone)
{
	return zone->pg_offset + bitmap_first_clear(zone->free_pages, PAGE_COUNT);
}

static void set_free_pg(zone_t *zone, pg_idx_t pg_to_free)
{
	pg_idx_t adjusted_pg_idx = pg_to_free - zone->pg_offset;
	bool is_used = bitmap_read(zone->free_pages, adjusted_pg_idx);
	
	if (!is_used) {
		kpanic();	
	}

	bitmap_clear(zone->free_pages, adjusted_pg_idx);
}

static void mark_page_used(zone_t *zone, pg_idx_t pg_idx)
{
	bitmap_set(zone->free_pages, pg_idx);
}

static pg_idx_t mark_pg_used_linear(void)
{
	zone_t *zone_linear = get_zone_linear();
	pg_idx_t pg_idx = get_free_pg(zone_linear);

	if (pg_idx == NOT_FOUND) {
		return NOT_FOUND;
	}
	
	fr_idx_t fr_idx = ZONE_LINEAR_FRAME_IDX(pg_idx);
	frame_t *frame = alloc_frame(fr_idx);	

	if (frame == NULL) {
		kpanic();
	}

	mark_page_used(zone_linear, fr_idx);
	return pg_idx;
}

static void mark_pg_free_linear(pg_idx_t pg_idx)
{
	zone_t *zone_linear = get_zone_linear();
	set_free_pg(zone_linear, pg_idx);
	
	fr_idx_t fr_idx = ZONE_LINEAR_FRAME_IDX(pg_idx);
	frame_t *frame = free_frame(fr_idx);	

	if (frame == NULL) {
		kpanic();
	}
}

static pg_idx_t alloc_pg_table(pg_idx_t pg_idx)
{
	pg_idx_t pg_table_idx = mark_pg_used_linear();

	if (pg_table_idx == NOT_FOUND) {
		return NOT_FOUND;
	}

	fr_idx_t fr_table_idx = ZONE_LINEAR_FRAME_IDX(pg_table_idx);
	pg_idx_t tmp_pg_table_idx = alloc_tmp_pg(fr_table_idx << PAGE_ORDER);
	pte_t *tmp_pg_table = (pte_t *)PAGE_VA(tmp_pg_table_idx);

	fr_idx_t fr_idx = ZONE_LINEAR_FRAME_IDX(pg_idx);
	fr_idx_t start_fr = fr_idx & ~BITMASK(PTE_ORDER);
	map_linear_pg_table(tmp_pg_table, start_fr);

	free_tmp_pg(tmp_pg_table_idx);
	map_pde_for_pg(pg_idx, fr_table_idx);
	return pg_idx;
}

pg_idx_t alloc_linear(void)
{
	pg_idx_t pg_idx = mark_pg_used_linear();

	if (pg_idx == NOT_FOUND) {
		return NOT_FOUND;
	}

	pde_t pde = read_pde_for_pg(pg_idx);

	if (!IS_PRESENT(pde)) {
		alloc_pg_table(pg_idx);
		pde = read_pde_for_pg(pg_idx);
		return pg_idx;
	}

	// assume that page table exists in linear
	fr_idx_t fr_idx = ZONE_LINEAR_FRAME_IDX(pg_idx);
	map_pte_for_pg(pde, pg_idx, fr_idx);

	return pg_idx;
}

pg_idx_t free_linear(pg_idx_t pg_to_free)
{
	mark_pg_free_linear(pg_to_free);
	pde_t pde = read_pde_for_pg(pg_to_free);
	assert(IS_PRESENT(pde));
	unmap_pte_for_pg(pde, pg_to_free);

	// TODO: if the free request, sets the last page entry in page table to non-present
	// we can unmap the page that contains the page table mapping from page directory

	return pg_to_free;
}


frame_t *pg_linear_descriptor(va_t pg_va)
{
	pa_t page_frame_addr = __pa(pg_va);
	return frame_from_pa(page_frame_addr);
}
