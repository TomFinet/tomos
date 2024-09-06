#pragma once

#include <stdbool.h>

#include <memory/frame.h>
#include <memory/address.h>
#include <panic.h>

#define PAGE_NBYTES BIT(PAGE_ORDER)
#define PTE_COUNT BIT(PTE_ORDER)
#define PDE_COUNT BIT(PDE_ORDER)
#define PAGE_COUNT BIT(PDE_ORDER + PTE_ORDER)

/* Page flags */
#define PAGE_PRESENT(x) (x)
#define PAGE_RW(x)      ((x) << 0x01)
#define PAGE_US(x)      ((x) << 0x02)
#define PAGE_PWT(x)     ((x) << 0x03)
#define PAGE_PCD(x)     ((x) << 0x04)
#define PAGE_A(x)       ((x) << 0x05)
#define PAGE_D(x)       ((x) << 0x06)
#define PAGE_PAT(x)     ((x) << 0x07)
#define PAGE_G(x)       ((x) << 0x08)

#define PAGE_STD(pa) ((pa) | PAGE_PRESENT(1) | PAGE_RW(1))

#define PAGE_DIR_IDX(va)   ((va) >> 22)
#define PAGE_TABLE_IDX(va) (0xff & ((va) >> 12))

/* Page flag checks */
#define PAGE_PA(pte)    ((pte) & ~0xfff)
#define IS_PRESENT(pte) ((pte) & 0x1)
#define IS_RW(pte)      ((pte) & 0x2)

#define PAGE_FREE_SUCCESS  1
#define PAGE_INVALID_BASE  -1
#define PAGE_ALREADY_FREE  -1
#define PAGE_IN_USE        -2

/* page table entry */
typedef uint32_t pte_t;

/* page directory entry */
typedef uint32_t pde_t;

void* page_alloc();
int page_free(va_t vbase);
struct frame_t* page_descriptor(void *va);
void page_init(void);
