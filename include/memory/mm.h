/*

MIT License

Copyright (c) 2023 Tom Finet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <klib/kstdio.h>

/* Kernel's physical memory manager, implementing single page frame allocation. */

#define SYSTEM_MEM 0x20000000 /* 512 MiB */

#define FRAME_NBYTES 4096
#define FRAME_SHIFT 12
#define FRAME_COUNT (SYSTEM_MEM >> FRAME_SHIFT)
#define FRAME_IDX(pa) (pa / FRAME_NBYTES)
#define FRAME_PA(idx) (FRAME_NBYTES * (idx))

#define MEM_MAP_BLOCK_NBITS	32
#define MEM_MAP_BLOCK_SHIFT 4
#define MEM_MAP_BLOCK_FULL 0xffffffff
#define MEM_MAP_BLOCK_COUNT (FRAME_COUNT >> MEM_MAP_BLOCK_SHIFT)

/// where the compile time parts of the kernel start and end in physical memory
extern uint32_t _kernel_physical_start;
extern uint32_t _kernel_physical_end;

void mm_init(void);

/// returns the start physical address of the free frame
uint32_t alloc_frame();

/// we could accidentally have duplicate frames
void free_frame(uint32_t frame_paddr);