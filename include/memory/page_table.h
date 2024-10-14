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
pte_t read_pte(pde_t pde, pg_idx_t pg_idx);
pde_t map_pde_for_pg(pg_idx_t pg_idx, fr_idx_t fr_idx);
pte_t map_pte_for_pg(pde_t pde, pg_idx_t pg_idx, fr_idx_t fr_idx);
pte_t map_pte(pte_t *pg_table, unsigned int pte_idx, fr_idx_t fr_idx);
void map_linear_pg_table(pte_t *pg_table, fr_idx_t fr_idx);
pte_t unmap_pte_for_pg(pde_t pde, pg_idx_t pg_idx);
pte_t unmap_pte(pte_t *pg_table, pg_idx_t pg_idx);

void invlpg(pg_idx_t pg_idx);
void flush_tlb(void);
