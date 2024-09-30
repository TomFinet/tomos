#include <memory/page_table.h>
#include <ksymbol.h>

SYMBOL_DEFINE(page_dir, pde_t *);

pde_t read_pde(pg_idx_t pg_idx)
{
	int pde_idx = PDE_IDX(pg_idx);
	pde_t pde = page_dir[pde_idx];
	return pde;
}

pte_t *get_pg_table(pde_t pde, pg_idx_t pg_idx)
{
	pa_t pg_table_pa = PAGE_PA(pde);	
	pte_t *pg_table = (pte_t *)__va(pg_table_pa);
	return pg_table;
}

pte_t read_pte(pde_t pde, pg_idx_t pg_idx)
{
	int pte_idx = PTE_IDX(pg_idx);
	pte_t *pg_table = get_pg_table(pde, pg_idx);
	return pg_table[pte_idx];
}

/*
 * Maps a page table into the page directory which is
 * needed to map the page at index pg_idx.
 *
 * Returns the virtual address of the page table that has
 * just been mapped.
 */
pde_t map_pde_for_pg(pg_idx_t pg_idx, fr_idx_t fr_idx)
{
	int pde_idx = PDE_IDX(pg_idx);
	pa_t fr_pa = fr_idx << FRAME_ORDER;
	page_dir[pde_idx] = PAGE_STD(fr_pa);
	return page_dir[pde_idx];
}

pte_t map_pte_for_pg(pde_t pde, pg_idx_t pg_idx, fr_idx_t fr_idx)
{
	pte_t *pg_table = get_pg_table(pde, pg_idx);
	int pte_idx = PTE_IDX(pg_idx);
	pa_t fr_pa = fr_idx << FRAME_ORDER;
	// TODO: assert that page is not-present
	pg_table[pte_idx] = PAGE_STD(fr_pa);
	invlpg(pg_idx);
	return pg_table[pte_idx];
}

pte_t unmap_pte_for_pg(pde_t pde, pg_idx_t pg_idx)
{
	pte_t *pg_table = get_pg_table(pde, pg_idx);
	int pte_idx = PTE_IDX(pg_idx);
	// TODO: assert that page is present
	pg_table[pte_idx] &= ~PAGE_PRESENT(1);
	invlpg(pg_idx);
	return pg_table[pte_idx];
}

void invlpg(pg_idx_t pg_idx)
{
	va_t pg_va = PAGE_VA(pg_idx);
	asm("invlpg %0" : : "m"(pg_va) : "memory");
}
