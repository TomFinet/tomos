#pragma once

#include <klib/kbit.h>
#include <klib/kbitmap.h>

#include <memory/memory_def.h>
#include <memory/paging.h>

typedef struct zone_struct {
	pg_idx_t base_page_idx;
	unsigned long num_pages;
	unsigned long pg_offset;
	BITMAP(free_pages, PAGE_COUNT);
} zone_t;

#define ZONE_INIT(base, pages, offset)	\
	{				\
		.base_page_idx = base,	\
		.num_pages = pages,	\
		.pg_offset = offset	\
	}

/*
 * The linear zone has the page table mapping
 *
 * 	va = pa + KERNEL_VA_BASE
 *
 * The kernel operates with linear mappings to avoid
 * walking the page table when translating va to pa.
 * The offset is chosen so that user processes that
 * start at va = 0 are supported because our kernel
 * is out of the way in the top quarter of va memory.
 */
#define ZONE_LINEAR_VA_BASE		KERNEL_VA_BASE
#define ZONE_LINEAR_NBYTES		MiB(800)
#define ZONE_LINEAR_BASE_PAGE_IDX	(ZONE_LINEAR_VA_BASE >> PAGE_ORDER)
#define ZONE_LINEAR_NUM_PAGES		(ZONE_LINEAR_NBYTES >> PAGE_ORDER)

#define ZONE_LINEAR_INIT 			\
	ZONE_INIT(ZONE_LINEAR_BASE_PAGE_IDX,	\
		ZONE_LINEAR_NUM_PAGES,		\
		ZONE_LINEAR_BASE_PAGE_IDX)

#define ZONE_LINEAR_FRAME_IDX(pg_idx) ((pg_idx) - ZONE_LINEAR_BASE_PAGE_IDX)

/*
 * The adhoc zone maps pages arbitrarily.
 * It is used to access outside the kernel
 * portion of pa, such as the process space.
 */
#define ZONE_ADHOC_VA_BASE		(ZONE_LINEAR_VA_BASE + ZONE_LINEAR_NBYTES)
#define ZONE_ADHOC_NBYTES		(KERNEL_VA_TOP - ZONE_ADHOC_VA_BASE + 1)
#define ZONE_ADHOC_BASE_PAGE_IDX	(ZONE_ADHOC_VA_BASE >> PAGE_ORDER)
#define ZONE_ADHOC_NUM_PAGES		(ZONE_ADHOC_NBYTES >> PAGE_ORDER)

#define ZONE_ADHOC_INIT				\
	ZONE_INIT(ZONE_ADHOC_BASE_PAGE_IDX,	\
		ZONE_ADHOC_NUM_PAGES,		\
		ZONE_ADHOC_BASE_PAGE_IDX)

zone_t *get_zone_linear(void);
zone_t *get_zone_adhoc(void);
void zone_mark_pages_used(
		zone_t *zone_linear,
		pg_idx_t pg_start,
		pg_idx_t pg_end);
