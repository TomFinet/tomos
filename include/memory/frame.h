#pragma once
#include <stdbool.h>

#include <memory/address.h>
#include <memory/memory_def.h>

#include <klib/kstdio.h>
#include <list.h>

#include <stddef.h>

/* Kernel's physical memory manager.
 * Implements single page frame allocation. */

#define FRAME_ORDER   12
#define FRAME_NBYTES  ORDER_SHL(FRAME_ORDER)
#define FRAME_COUNT   (SYSTEM_MEM >> FRAME_ORDER)

/* Physical frame descriptor. */
struct frame_t {
	struct kslab_t *slab;
	uint32_t flags;
};

void frame_init(void);

/* returns the start physical address of the free frame. */
pa_t alloc_frame(void);
void free_frame(pa_t frame);
struct frame_t* pa_to_frame(pa_t frame);
bool is_frame_free(int idx);
