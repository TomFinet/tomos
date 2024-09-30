#pragma once

#include <memory/frame.h>
#include <memory/address.h>

#include <klib/kbit.h>

typedef int pg_idx_t;

#define PAGE_VA(pidx)	((pidx) << PAGE_ORDER)
#define PAGE_IDX(va)	((va) >> PAGE_ORDER)

/* Page flag checks */

#define PAGE_FREE_SUCCESS 1
#define PAGE_ALREADY_FREE -1

void *page_alloc();
int page_free(va_t vbase);
frame_t *page_descriptor(void *va);
void page_init(void);
