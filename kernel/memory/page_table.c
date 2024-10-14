#include <memory/page_table.h>
#include <memory/pg_tmp.h>
#include <memory/zone.h>
#include <ksymbol.h>

SYMBOL_PA_DEFINE(page_dir, pde_t *);

void write_pde(unsigned int pde_idx, pde_t pde)
{
	page_dir[pde_idx] = pde;
}

pde_t read_pde_for_pg(pg_idx_t pg_idx)
{
	int pde_idx = PDE_IDX(pg_idx);
	return read_pde(pde_idx);
}

pde_t read_pde(unsigned int pde_idx)
{
	return page_dir[pde_idx];
}

pte_t read_pte(pde_t pde, pg_idx_t pg_idx)
{
	int pte_idx = PTE_IDX(pg_idx);
	pg_idx_t pg_table_pg_idx = alloc_tmp_pg(PAGE_PA(pde));
	pte_t *pg_table = (pte_t *)PAGE_VA(pg_table_pg_idx);
	pte_t pte = pg_table[pte_idx];
	free_tmp_pg(pg_table_pg_idx);
	return pte;
}

pde_t map_pde_for_pg(pg_idx_t pg_idx, fr_idx_t fr_idx)
{
	int pde_idx = PDE_IDX(pg_idx);
	pa_t fr_pa = fr_idx << FRAME_ORDER;
	page_dir[pde_idx] = PAGE_STD(fr_pa);
	flush_tlb();
	return page_dir[pde_idx];
}

pte_t map_pte_for_pg(pde_t pde, pg_idx_t pg_idx, fr_idx_t fr_idx)
{
	pg_idx_t pg_table_pg_idx = alloc_tmp_pg(PAGE_PA(pde));
	pte_t *pg_table = (pte_t *)PAGE_VA(pg_table_pg_idx);
	unsigned int pte_idx = PTE_IDX(pg_idx);
	pte_t pte = map_pte(pg_table, pte_idx, fr_idx);
	free_tmp_pg(pg_table_pg_idx);
	flush_tlb();
	return pte;
}

pte_t map_pte(pte_t *pg_table, unsigned int pte_idx, fr_idx_t fr_idx)
{
	pa_t fr_pa = fr_idx << FRAME_ORDER;
	pg_table[pte_idx] = PAGE_STD(fr_pa);
	return pg_table[pte_idx];
}

pte_t unmap_pte_for_pg(pde_t pde, pg_idx_t pg_idx)
{
	pg_idx_t pg_table_pg_idx = alloc_tmp_pg(PAGE_PA(pde));
	pte_t *pg_table = (pte_t *)PAGE_VA(pg_table_pg_idx);
	pte_t pte = unmap_pte(pg_table, pg_idx);
	free_tmp_pg(pg_table_pg_idx);
	return pte;
}

pte_t unmap_pte(pte_t *pg_table, pg_idx_t pg_idx)
{
	int pte_idx = PTE_IDX(pg_idx);
	pg_table[pte_idx] = 0;
	flush_tlb();
	return pg_table[pte_idx];
}

void map_linear_pg_table(pte_t *pg_table, fr_idx_t fr_idx)
{
	// assert fr_idx is aligned with start of pg table
	for (unsigned int pte_idx = 0; pte_idx < PTE_COUNT; pte_idx++) {
		pa_t fr_pa = (fr_idx + pte_idx) << PAGE_ORDER;
		pte_t pte = PAGE_STD(fr_pa);
		pg_table[pte_idx] = pte;
	}
}

void invlpg(pg_idx_t pg_idx)
{
	va_t pg_va = PAGE_VA(pg_idx);
	asm("invlpg %0" : : "m"(pg_va) : "memory");
}

void flush_tlb(void)
{
	pa_t page_dir_pa = ((va_t)page_dir) - ZONE_LINEAR_VA_BASE;
	asm("movl %0, %%cr3": : "r"(page_dir_pa) : "memory");
}
