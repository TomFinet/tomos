#pragma once

#include <stdbool.h>

#include <memory/frame.h>
#include <memory/address.h>
#include <panic.h>

#define PAGE_ORDER 12
#define PTE_ORDER 10
#define PAGE_NBYTES 4096
#define PTE_COUNT   1024
#define KERNEL_START_PAGE_NUM (KERNEL_BASE >> PAGE_ORDER)

#define PAGE_FREE_MAP_SIZE (1 << 20)

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

#define PAGE_PA(pte)        ((pte) & ~0xfff)
#define PAGE_DIR_IDX(pte)   ((pte) >> 22)
#define PAGE_TABLE_IDX(pte) (0xff & ((pte) >> 12))

/* Page flag checks */
#define IS_PRESENT(pte) ((pte) & 0x1)
#define IS_RW(pte)      ((pte) & 0x2)
#define IS_ALIGNED(pte) (!((pte) & 0xfff))

#define PAGE_FREE_SUCCESS  1
#define PAGE_INVALID_BASE  -1
#define PAGE_ALREADY_FREE  -1
#define PAGE_IN_USE        -2

enum page_type { USER, KERNEL };

/* page table entry */
typedef uint32_t pte_t;

/* page directory entry */
typedef uint32_t pde_t;

/* where the kernel starts in virtual memory,
defined in the linker script. */
extern va_t _higher_start;

extern pa_t _page_dir;

void page_init(void);

void* page_alloc(enum page_type type);
void* kpage_alloc(void);

/* unmaps a page beginning at vbase */
int page_free(va_t vbase);

/* reads the page table entry at vbase. */
pte_t page_table_read(va_t vbase);

/* Returns the page descriptor for a given va. */
struct frame_t *page_descriptor(void *va);
