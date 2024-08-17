#include <memory/frame.h>
#include <klib/kbitmap.h>
#include <klib/kstring.h>

/* Array of frame descriptors. Can be quite large. */
static struct frame_t frames[FRAME_COUNT];
static bool init_done = false;

/* Bitmap tracking free/used page frames in physical memory. */
BITMAP(mem_map, FRAME_COUNT);

bool is_frame_free(int frame_pos)
{
	return bitmap_read(mem_map, frame_pos) == 0;
}

void frame_init(void)
{
	if (init_done) {
		return;
	}

	/* 0x0 - _kernel_physical_end already allocated */
	int kernel_frame_cnt =
		((uint32_t)&_kernel_physical_end + FRAME_NBYTES - 1) /
		FRAME_NBYTES;
	int bitmap_blks_needed = BITMAP_BLKS(kernel_frame_cnt);

	for (int b = 0; b < bitmap_blks_needed; b++) {
		mem_map[b] = ALL_SET;
		/* TODO: initialise the frame struct list? */
	}
	
	int part_blk_idx = BITMAP_POS(kernel_frame_cnt);
	if (part_blk_idx) {
		mem_map[bitmap_blks_needed - 1] &= (1 << part_blk_idx) - 1;
	}
	init_done = true;
}

uint32_t alloc_frame(void)
{
	int clear_bit_pos = bitmap_first_clear(mem_map, FRAME_COUNT);
	if (clear_bit_pos == NOT_FOUND) {
		return (uint32_t)NULL;
	}
	bitmap_set(mem_map, clear_bit_pos);
	return FRAME_PA(clear_bit_pos);
}

void free_frame(uint32_t pa)
{
	bitmap_clear(mem_map, FRAME_IDX(pa));
}

struct mem_map_idx_t pa_to_idx(uint32_t pa)
{
	struct mem_map_idx_t idx = {
		.block = FRAME_IDX(pa) / 32,
		.idx = FRAME_IDX(pa) % 32,
	};
	return idx;
}

struct frame_t *pa_to_frame(uint32_t pa)
{
	return &frames[FRAME_IDX(pa)];
}
