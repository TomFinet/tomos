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

#include <panic.h>
#include <memory/mm.h>

#define KERNEL_BASE 	    0xC0000000  // 3GiB
#define PTE_COUNT	        1024
#define PAGE_NBYTES         4096        // same as frame size

#define __pa(va) ((va) - KERNEL_BASE) 
#define __va(pa) ((pa) + KERNEL_BASE)

/// Page flags
#define PAGE_PRESENT(x)		(x)
#define PAGE_RW(x) 			((x) << 0x01)
#define PAGE_US(x) 			((x) << 0x02)
#define PAGE_PWT(x)			((x) << 0x03)
#define PAGE_PCD(x)			((x) << 0x04)
#define PAGE_A(x)			((x) << 0x05)
#define PAGE_D(x)			((x) << 0x06)
#define PAGE_PAT(x)			((x) << 0x07)
#define PAGE_G(x)			((x) << 0x08)

#define PAGE_STD(pa)        ((pa) | PAGE_PRESENT(1) | PAGE_RW(1))

#define PAGE_PA(pte) 		((pte) & ~0xfff)
#define PAGE_DIR_IDX(pte) 	((pte) >> 22)
#define PAGE_TABLE_IDX(pte) (0xfff & ((pte) >> 12))

/// Page flag checks
#define IS_PRESENT(pte)		((pte) & 0x1)
#define IS_RW(pte)			((pte) & 0x2)
#define IS_ALIGNED(pte)     (!((pte) & 0xfff))

#define PAGE_ALLOC_SUCCESS   1
#define PAGE_FREE_SUCCESS    1
#define PAGE_INVALID_BASE   -1
#define PAGE_ALREADY_FREE   -1
#define PAGE_IN_USE         -2

/// page table entry
typedef uint32_t pte_t;

/// page directory entry
typedef uint32_t pde_t;

/// a physical address
typedef uint32_t pa_t;

/// a virtual address
typedef uint32_t va_t;

/// where the kernel starts in virtual memory
/// defined in the linker script.
extern va_t _higher_start;

extern pa_t _page_dir;

void page_init(void);

/// maps a page beginning at vbase into the physical address space
/// returns a negative int on fail, and a non-negative int on success.
int32_t page_alloc(va_t vbase);

/// unmaps a page beginning at vbase
int32_t page_free(va_t vbase);