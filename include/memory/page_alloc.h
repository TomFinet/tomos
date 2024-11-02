#pragma once

#include <memory/address.h>
#include <memory/frame.h>
#include <memory/paging.h>

/*
 * Allocates a page of virtual memory which is mapped linearly
 * to physical memory.
 *
 * Linear zone guarantees that a page can be mapped to a single frame,
 * and a frame can be mapped to a single page. So checking that the
 * zone has a free page, tells us that it must have a free frame.
 * This is not true of other zones (e.g. if virtual mem >> phys mem)
 *
 */
pg_idx_t alloc_linear(void);
pg_idx_t free_linear(pg_idx_t pg_to_free);

frame_t *pg_linear_descriptor(va_t pg_va);

void zero_pg(pg_idx_t pg_idx);
