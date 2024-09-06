#include <memory/frame.h>

#include <klib/kstring.h>

#include <ksymbol.h>
#include <panic.h>

/* where the static parts of the kernel
start and end in physical memory. */
extern pa_t _kernel_pa_end;

/* Array of frame descriptors. Can be quite large. */
static struct frame_t frames[FRAME_COUNT];

/* Tracks free/used page frames in physical memory. */
BITMAP(mem_map, FRAME_COUNT);

static bool init_done = false;
void frame_init(void)
{
	if (init_done) {
		return;
	}

	/* 0x0 - _kernel_physical_end already allocated */
	pa_t kernel_end = SYMBOL_READ(_kernel_pa_end, pa_t);
	int kernel_frame_cnt = (kernel_end + FRAME_NBYTES - 1) / FRAME_NBYTES;
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

pa_t frame_alloc(void)
{
	int frame_free_idx = bitmap_first_clear(mem_map, FRAME_COUNT);
	if (frame_free_idx == NOT_FOUND) {
		kpanic();
	}
	bitmap_set(mem_map, frame_free_idx);
	return frame_free_idx << FRAME_ORDER;
}

void frame_free(pa_t frame)
{
	bitmap_clear(mem_map, frame >> FRAME_ORDER);
}

struct frame_t* frame_from_pa(pa_t frame)
{
	return &frames[frame >> FRAME_ORDER];
}

bool frame_is_free(int frame_pos)
{
	return bitmap_read(mem_map, frame_pos) == 0;
}
