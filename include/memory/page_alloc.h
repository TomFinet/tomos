#pragma once

#include <memory/address.h>
#include <memory/frame.h>
#include <memory/paging.h>

/* Allocate a single page in the linear address zone. */
pg_idx_t alloc_linear(void);
pg_idx_t free_linear(pg_idx_t pg_to_free);

pg_idx_t alloc_pg_table(pg_idx_t pg_idx);

va_t alloc_adhoc(void);

frame_t *pg_linear_descriptor(va_t pg_va);

void zero_pg(pg_idx_t pg_idx);
