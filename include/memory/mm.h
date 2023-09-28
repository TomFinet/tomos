#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <klib/kstdio.h>
#include <list.h>

/* Kernel's physical memory manager, implementing single page frame allocation.
 */

#define SYSTEM_MEM 0x20000000 /* 512 MiB */

#define FRAME_NBYTES  4096
#define FRAME_SHIFT   12
#define FRAME_COUNT   (SYSTEM_MEM >> FRAME_SHIFT)
#define FRAME_IDX(pa) ((pa) / FRAME_NBYTES)
#define FRAME_PA(idx) (FRAME_NBYTES * (idx))

#define MEM_MAP_BLOCK_NBITS 32
#define MEM_MAP_BLOCK_SHIFT 4
#define MEM_MAP_BLOCK_FULL  0xffffffff
#define MEM_MAP_BLOCK_COUNT (FRAME_COUNT >> MEM_MAP_BLOCK_SHIFT)

/* Physical frame descriptor. */
struct frame_t {
	/* Can be a part of a slab. */
	struct kslab_t *slab;
	uint32_t flags;
};

struct mem_map_idx_t {
	uint32_t block;
	uint8_t idx;
};

/* where the static parts of the kernel
start and end in physical memory. */
extern uint32_t _kernel_physical_start;
extern uint32_t _kernel_physical_end;

void mm_init(void);

/* returns the start physical address of the free frame. */
uint32_t alloc_frame(void);

/* we could accidentally have duplicate frames. */
void free_frame(uint32_t pa);

struct mem_map_idx_t pa_to_idx(uint32_t pa);

struct frame_t *pa_to_frame(uint32_t pa);