#pragma once

#include <stdbool.h>

#include <memory/frame.h>
#include <memory/address.h>

#include <klib/kbit.h>
#include <panic.h>

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

#define PAGE_IDX(va) ((va) >> PAGE_ORDER)
#define PDE_IDX(va)  (PAGE_IDX(va) >> PTE_ORDER)
#define PTE_IDX(va)  (PAGE_IDX(va) & BITMASK(PTE_ORDER))

/* Page flag checks */
#define PAGE_PA(pte)    ((pte) & ~0xfff)
#define IS_PRESENT(pte) ((pte) & 0x1)
#define IS_RW(pte)      ((pte) & 0x2)

#define PAGE_FREE_SUCCESS 1
#define PAGE_ALREADY_FREE -1

/* page table entry */
typedef uint32_t pte_t;

/* page directory entry */
typedef uint32_t pde_t;

void *page_alloc();
int page_free(va_t vbase);
struct frame_t *page_descriptor(void *va);
void page_init(void);
