#pragma once

#include <stdbool.h>

#include <memory/address.h>
#include <memory/memory_def.h>

#include <klib/kbitmap.h>
#include <klib/kstdio.h>
#include <list.h>

#include <stddef.h>

typedef int fr_idx_t;

typedef struct frame_struct {
	struct kslab_t *slab;
	uint32_t flags;
} frame_t;

frame_t *alloc_frame(fr_idx_t fr_idx);
frame_t *free_frame(fr_idx_t fr_idx);

pa_t frame_alloc(void);
void frame_free(pa_t frame);
frame_t *frame_from_idx(fr_idx_t fr_idx);
frame_t *frame_from_pa(pa_t frame);
bool frame_is_free(int idx);
