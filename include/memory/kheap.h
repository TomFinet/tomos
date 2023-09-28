#pragma once

#include <list.h>
#include <stddef.h>

#include <memory/mm.h>
#include <memory/paging.h>

/* Tracks the page level kernel heap allocations. At the moment,
can only extend the heap one page at a time, no freeing. */

void kheap_init();

void *kheap_extend();