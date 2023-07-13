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

#include <memory/vmm.h>

/* First free vma. */
struct free_vma_t* free_start;

/* Top of the kernel heap. */
va_t kheap_top;

// Splits a vma at offset @split and returns a
// pointer to the start of the free memory area.
static va_t split_free_vma(struct free_vma_t* vma, uint32_t split) {
	struct free_vma_t* updated = (struct free_vma_t *) (((va_t) (vma + 1)) + vma->size);
	updated->size = vma->size - split - sizeof(struct free_vma_t);
	
	if(vma->list.next) {
		updated->list.next = vma->list.next;
	}

	vma->list.next = (struct list_head *) updated;
	list_remove(&vma->list);

	return (va_t) (vma + 1);
}

// Extends the kernel heap by one page.
static va_t page_extend_kheap() {
	page_alloc(kheap_top);
	va_t page = kheap_top;
	kheap_top += PAGE_NBYTES;
	return page;
}

void vmm_init() {
	// allocate the first kernel heap page.
	page_extend_kheap();

	// memory is managed with an implicit free list, i.e. each
	// free vma has a header pointing to the next and previous vma's.
	free_start = (struct free_vma_t *) _kheap_start;
	free_start->size = 0;
	free_start->list.next = NULL;
	free_start->list.prev = NULL;
}

/// TODO: assumes the value of @size is less than a page
va_t vmm_alloc_kheap(uint32_t size) {
	struct free_vma_t* curr = free_start;
	while(curr->list.next != NULL) {
		curr = list_entry(curr->list.next, struct free_vma_t, list);
		
		if(curr->size >= size) {
			return split_free_vma(curr, size);
		}
	}

	// could not find a free vma of sufficient
	// size, try to allocate a new page.
	// TODO: try rearranging the vma's to create more space
	va_t pg_start = page_extend_kheap();
	struct free_vma_t* header = (struct free_vma_t *) pg_start;
	header->size = size;
	header->list.next = NULL;
	header->list.prev = (struct list_head *) curr;
	curr->list.next = (struct list_head *) header;

	return pg_start + header->size;
}