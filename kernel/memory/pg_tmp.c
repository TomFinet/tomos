#include <memory/pg_tmp.h>
#include <memory/paging.h>
#include <memory/frame.h>
#include <memory/memory_def.h>

#include <ksymbol.h>

// Virtual address of the temporary page table which exists in linear memory.
SYMBOL_PA_DEFINE(tmp_pg_table, pte_t *);

static unsigned int map_tmp_pg(fr_idx_t fr_idx)
{
	for (unsigned int pte_idx = 0; pte_idx < PTE_COUNT; pte_idx++) {
		if (!IS_PRESENT(tmp_pg_table[pte_idx])) {
			map_pte(tmp_pg_table, pte_idx, fr_idx);
			return pte_idx;
		}
	}
	return NOT_FOUND;
}

// do we need to mark any bits to set in our page free/used tracking bitmaps
pg_idx_t alloc_tmp_pg(pa_t pa)
{
	fr_idx_t fr_idx = PAGE_IDX(pa);
	unsigned int tmp_pte_idx = map_tmp_pg(fr_idx);

	if (tmp_pte_idx == NOT_FOUND) {
		return NOT_FOUND;
	}

	pg_idx_t tmp_pg_idx = (TMP_PG_TABLE_PDE << PTE_ORDER) + tmp_pte_idx;
	return tmp_pg_idx;
}

pte_t free_tmp_pg(pg_idx_t pg_idx)
{
	// assert pg is in tmp range
	return unmap_pte(tmp_pg_table, pg_idx);
}

void reset_tmp_pg_table(void)
{
	memset(tmp_pg_table, 0, PTE_COUNT * sizeof(pte_t));
}
