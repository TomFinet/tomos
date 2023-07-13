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

#include <stddef.h>

#include <list.h>
#include <memory/paging.h>

/* Kernel's virtual memory manager, implementing a crappy kmalloc. */

// TODO: optimise virtual memory allocation for networking.

// Descrribes a free contiguous virtual memory area
// (vma), embedded in memory at the start of each vma.
// size refers to the number of bytes in the vma
// excluding this header.
struct free_vma_t {
	uint32_t size;
	struct list_head list;
};

/* Dynamically allocated kernel virtual memory is managed starting from here. */
extern va_t _kheap_start __attribute__((aligned(PAGE_NBYTES)));

void vmm_init();

/// Allocate a contiguous block of memory
/// in kernel space of the specified size.
va_t vmm_alloc_kheap(uint32_t size);