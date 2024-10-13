#include <memory/page_table.h>
#include <memory/zone.h>
#include <ksymbol.h>

SYMBOL_PA_DEFINE(page_dir, pde_t *);
// Virtual address of the temporary page table which exists in linear memory.
SYMBOL_PA_DEFINE(tmp_pg_table, pte_t *);

void write_pde(unsigned int pde_idx, pde_t pde)
{
	page_dir[pde_idx] = pde;
}

pde_t read_pde(unsigned int pde_idx) {
	return page_dir[pde_idx];
}

pde_t read_pde_for_pg(pg_idx_t pg_idx)
{
	int pde_idx = PDE_IDX(pg_idx);
	pde_t pde = page_dir[pde_idx];
	return pde;
}

pte_t *get_tmp_mapping(pde_t pde)
{
	pa_t pa = PAGE_PA(pde);
	fr_idx_t fr_idx = PAGE_IDX(pa);
	unsigned int tmp_pte_idx = map_tmp_pg(fr_idx);

	if (tmp_pte_idx == NOT_FOUND) {
		return NULL;
	}

	va_t tmp_pg_va = (5 << (PAGE_ORDER + PTE_ORDER)) + (tmp_pte_idx << PAGE_ORDER);
	return (void *)tmp_pg_va;
}

pte_t read_pte(pde_t pde, pg_idx_t pg_idx)
{
	int pte_idx = PTE_IDX(pg_idx);
	pte_t *pg_table = get_tmp_mapping(pde);
	pte_t pte = pg_table[pte_idx];
	unmap_tmp_pg(PAGE_IDX((va_t)pg_table));
	return pte;
}

unsigned int map_tmp_pg(fr_idx_t fr_idx)
{
	for (unsigned int pte_idx = 0; pte_idx < PTE_COUNT; pte_idx++) {
		if (!IS_PRESENT(tmp_pg_table[pte_idx])) {
			map_pte(tmp_pg_table, pte_idx, fr_idx);
			return pte_idx;
		}
	}
	return NOT_FOUND;
}

pte_t unmap_tmp_pg(pg_idx_t pg_idx)
{
	// assert pg is in tmp range
	return unmap_pte(tmp_pg_table, pg_idx);
}

// After startup the tmp page table is set from the
// identity page table, so mark each pte as free.
void reset_tmp_pg_table(void)
{
	memset(tmp_pg_table, 0, PTE_COUNT * sizeof(pte_t));
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
	pte_t *pg_table = get_tmp_mapping(pde);
	unsigned int pte_idx = PTE_IDX(pg_idx);
	pte_t pte = map_pte(pg_table, pte_idx, fr_idx);
	unmap_tmp_pg(PAGE_IDX((va_t)pg_table));
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
	pte_t *pg_table = get_tmp_mapping(pde);
	pte_t pte = unmap_pte(pg_table, pg_idx);
	unmap_tmp_pg(PAGE_IDX((va_t)pg_table));
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
