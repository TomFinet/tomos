#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <klib/kstdio.h>
#include <memory/address.h>
#include <list.h>

/* Kernel's physical memory manager, implementing single page frame allocation.
 */

#define SYSTEM_MEM 0x20000000 /* 512 MiB */

#define FRAME_NBYTES  4096
#define FRAME_ORDER   12
#define FRAME_COUNT   (SYSTEM_MEM >> FRAME_ORDER)

#define MEM_MAP_BLOCK_SHIFT 4
#define MEM_MAP_BLOCK_COUNT (FRAME_COUNT >> MEM_MAP_BLOCK_SHIFT)

/* Physical frame descriptor. */
struct frame_t {
	struct kslab_t *slab;
	uint32_t flags;
};

struct mem_map_idx_t {
	uint32_t block;
	uint8_t idx;
};

/* where the static parts of the kernel
start and end in physical memory. */
extern pa_t _kernel_physical_start;
extern pa_t _kernel_physical_end;

void frame_init(void);

/* returns the start physical address of the free frame. */
pa_t alloc_frame(void);

/* we could accidentally have duplicate frames. */
void free_frame(pa_t pa);

bool is_frame_free(int frame_pos);

struct mem_map_idx_t pa_to_idx(pa_t pa);

struct frame_t* pa_to_frame(pa_t pa);
