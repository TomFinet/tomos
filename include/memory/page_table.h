#pragma once

#include <klib/kbit.h>

#include <memory/paging.h>

#include <stdint.h>

/* Page flags */
#define PAGE_PRESENT(x) BITSET(x, 0)
#define PAGE_RW(x)      BITSET(x, 1)
#define PAGE_US(x)      BITSET(x, 2)
#define PAGE_PWT(x)     BITSET(x, 3)
#define PAGE_PCD(x)     BITSET(x, 4)
#define PAGE_A(x)       BITSET(x, 5)
#define PAGE_D(x)       BITSET(x, 6)
#define PAGE_PAT(x)     BITSET(x, 7)
#define PAGE_G(x)       BITSET(x, 8)

#define PAGE_STD(pa) ((pa) | PAGE_PRESENT(1) | PAGE_RW(1))

#define PAGE_PA(pte)    ((pte) & ~0xfff)
#define IS_PRESENT(pte) BITGET(pte, 0)
#define IS_RW(pte)      BITGET(pte, 1)

#define PDE_IDX(pidx)	((pidx) >> PTE_ORDER)
#define PTE_IDX(pidx)	((pidx) & BITMASK(PTE_ORDER))

/* page table entry */
typedef uint32_t pte_t;

/* page directory entry */
typedef uint32_t pde_t;

void write_pde(unsigned int pde_idx, pde_t pde);
pde_t read_pde(unsigned int pde_idx);

pde_t read_pde_for_pg(pg_idx_t pg_idx);
pte_t *get_tmp_mapping(pde_t pde);
pte_t read_pte(pde_t pde, pg_idx_t pg_idx);
pde_t map_pde_for_pg(pg_idx_t pg_idx, fr_idx_t fr_idx);
pte_t map_pte_for_pg(pde_t pde, pg_idx_t pg_idx, fr_idx_t fr_idx);
pte_t map_pte(pte_t *pg_table, unsigned int pte_idx, fr_idx_t fr_idx);
pte_t unmap_pte_for_pg(pde_t pde, pg_idx_t pg_idx);
pte_t unmap_pte(pte_t *pg_table, pg_idx_t pg_idx);


void reset_tmp_pg_table(void);

/*
 * When we allocate a new page table, we need to write to it to make mappings.
 * But to write to it, we need to have it mapped, which may not be the case.
 * To get around this chicken and egg scenario, we reserve a page table to
 * temporarily map these page tables into a startup reserved page table.
 *
 * Map pg_idx into fr_idx in the temporary page table
 *
 * TODO: this is slow, O(PTE_COUNT) which is very bad when it is called PTE_COUNT times
 * i.e. O(PTE_COUNT^2) and PTE_COUNT = 1024 so 1024^2 = 1 million ops
 */
unsigned int map_tmp_pg(fr_idx_t fr_idx);
pte_t unmap_tmp_pg(pg_idx_t pg_idx);

void invlpg(pg_idx_t pg_idx);
void flush_tlb(void);
