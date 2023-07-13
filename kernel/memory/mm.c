#include <memory/mm.h>

/// bit map storing which frames are free
/// TODO: will likely have to modify the approach when
/// 	  we start to store permission bits for frames
uint32_t mem_map[MEM_MAP_BLOCK_COUNT];

bool inline is_frame_free(uint32_t block, uint8_t idx) {
	return (mem_map[block] >> idx) & 0x1;
}

/* maps the frame idx into the memory map */
void inline map_frame(uint32_t block, uint8_t idx) {
	mem_map[block] |= 0x1 << idx;
}

/* unmaps the frame number idx into the memory map */
void inline unmap_frame(uint32_t block, uint8_t idx) {
	mem_map[block] &= ~(0x1 << idx);
}

void mm_init(void) {
	/* 0x0 - _kernel_physical_end already allocated */
	int kernel_frame_cnt = ((uint32_t)&_kernel_physical_end + FRAME_NBYTES - 1) / FRAME_NBYTES;
	uint32_t full_blocks = kernel_frame_cnt / MEM_MAP_BLOCK_NBITS;
	uint32_t remaining = kernel_frame_cnt % MEM_MAP_BLOCK_NBITS; 

	for(int b = 0; b < full_blocks; b++) {
		mem_map[b] = MEM_MAP_BLOCK_FULL;
	}
	mem_map[full_blocks] |= (0x1 << remaining) - 1;
}

uint32_t alloc_frame() {
	for(uint32_t block = 0; block < MEM_MAP_BLOCK_COUNT; block++) {
		if(mem_map[block] == MEM_MAP_BLOCK_FULL) continue;
		uint8_t idx = __builtin_ctz(~mem_map[block]); 
		map_frame(block, idx);
		return FRAME_PA(MEM_MAP_BLOCK_NBITS * block + idx);
	}
	return (uint32_t) NULL; /* frame alloc failed */
}

void free_frame(uint32_t frame_paddr) {
	uint32_t fidx = FRAME_IDX(frame_paddr);
	uint32_t block = fidx / MEM_MAP_BLOCK_NBITS;
	uint8_t idx = fidx % MEM_MAP_BLOCK_NBITS; 
	unmap_frame(block, idx);
}
