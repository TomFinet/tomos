#include <memory/frame.h>

#include <klib/kstring.h>

#include <ksymbol.h>
#include <panic.h>

/* where the static parts of the kernel
start and end in physical memory. */
extern pa_t _kernel_pa_end;

/* Array of frame descriptors. Can be quite large. */
static frame_t frames[FRAME_COUNT];

/* Tracks free/used page frames in physical memory. */
BITMAP(mem_map, FRAME_COUNT);

frame_t *alloc_frame(fr_idx_t fr_idx)
{
	bool is_used = bitmap_read(mem_map, fr_idx);
	if (!is_used) {
		bitmap_set(mem_map, fr_idx);
		return frame_from_idx(fr_idx);
	}
	return NULL;
}

frame_t *free_frame(fr_idx_t fr_idx)
{
	bool is_used = bitmap_read(mem_map, fr_idx);
	if (is_used) {
		bitmap_clear(mem_map, fr_idx);
		return frame_from_idx(fr_idx);
	}
	return NULL;
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

frame_t *frame_from_idx(fr_idx_t fr_idx)
{
	return &frames[fr_idx];
}

frame_t *frame_from_pa(pa_t frame)
{
	return frame_from_idx(frame >> FRAME_ORDER);
}

bool frame_is_free(int frame_pos)
{
	return bitmap_read(mem_map, frame_pos) == 0;
}
