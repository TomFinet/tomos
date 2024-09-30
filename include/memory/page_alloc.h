#pragma once

#include <memory/address.h>
#include <memory/frame.h>
#include <memory/paging.h>

/* Allocate a single page in the linear address zone. */
va_t alloc_linear(void);
pg_idx_t free_linear(pg_idx_t pg_to_free);

va_t alloc_adhoc(void);

frame_t *pg_linear_descriptor(va_t pg_va);
