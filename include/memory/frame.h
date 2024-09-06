#pragma once
#include <stdbool.h>

#include <memory/address.h>
#include <memory/memory_def.h>

#include <klib/kbitmap.h>
#include <klib/kstdio.h>
#include <list.h>

#include <stddef.h>

/* Kernel's physical memory manager.
 * Implements single page frame allocation. */

#define FRAME_NBYTES BIT(FRAME_ORDER)
#define FRAME_COUNT (SYSTEM_MEM >> FRAME_ORDER)

/* Physical frame descriptor. */
struct frame_t {
	struct kslab_t *slab;
	uint32_t flags;
};

void frame_init(void);
pa_t frame_alloc(void);
void frame_free(pa_t frame);
struct frame_t* frame_from_pa(pa_t frame);
bool frame_is_free(int idx);
