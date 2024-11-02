#pragma once

#include <memory/address.h>
#include <memory/paging.h>
#include <memory/page_table.h>

// The pde idx that the temporary page table uses.
#define TMP_PG_TABLE_PDE (PDE_COUNT - 1)

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
pg_idx_t alloc_tmp_pg(pa_t pa);
pte_t free_tmp_pg(pg_idx_t pg_idx);

// After startup the tmp page table is set from the
// identity page table, so mark each pte as free.
void reset_tmp_pg_table(void);
