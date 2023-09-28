/* Use of this source code is governed by the MIT license that can be
found in the LICENSE file. */

#include <memory/mm.h>

/* Array of frame descriptors. Can be quite large. */
static struct frame_t frames[FRAME_COUNT];

/* Bitmap tracking free/used page frames in physical memory. */
uint32_t mem_map[MEM_MAP_BLOCK_COUNT];

bool inline is_frame_free(struct mem_map_idx_t i) {
	return (mem_map[i.block] >> i.idx) & 0x1;
}

void mm_init(void) {
	/* 0x0 - _kernel_physical_end already allocated */
	int kernel_frame_cnt = ((uint32_t)&_kernel_physical_end + FRAME_NBYTES - 1) / FRAME_NBYTES;
	uint32_t full_blocks = kernel_frame_cnt / MEM_MAP_BLOCK_NBITS;
	uint32_t remaining = kernel_frame_cnt % MEM_MAP_BLOCK_NBITS; 

	for(int b = 0; b < full_blocks; b++) {
		mem_map[b] = MEM_MAP_BLOCK_FULL;
    /* TODO: initialise the frame struct list? */
	}
	mem_map[full_blocks] |= (0x1 << remaining) - 1;
}

uint32_t alloc_frame(void) {
	for(uint32_t block = 0; block < MEM_MAP_BLOCK_COUNT; block++) {
		if(mem_map[block] == MEM_MAP_BLOCK_FULL) continue;
		uint8_t idx = __builtin_ctz(~mem_map[block]); 
    mem_map[block] |= (0x1 << idx); 
		return FRAME_PA(MEM_MAP_BLOCK_NBITS * block + idx);
	}
	return (uint32_t) NULL; /* frame alloc failed */
}

void free_frame(uint32_t pa) {
  struct mem_map_idx_t i = pa_to_idx(pa);
	mem_map[i.block] &= ~(0x1 << i.idx);
}

struct mem_map_idx_t pa_to_idx(uint32_t pa) {
	struct mem_map_idx_t idx = {
    .block = FRAME_IDX(pa) / MEM_MAP_BLOCK_NBITS,
	  .idx = FRAME_IDX(pa) % MEM_MAP_BLOCK_NBITS, 
  };
  return idx;
}

struct frame_t* pa_to_frame(uint32_t pa) {
  struct mem_map_idx_t i = pa_to_idx(pa);
  return &frames[MEM_MAP_BLOCK_NBITS * i.block + i.idx];
}
