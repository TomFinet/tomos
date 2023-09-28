#include <memory/kheap.h>

/* TODO: Track free/used virtual memory as vm_areas with page size
granularity. This enables us to free pages when we are done. */

extern va_t _kheap_start;

va_t kheap_top;

void kheap_init()
{
	kheap_top = (va_t)&_kheap_start;
}

/* Extends the kernel heap by one page. */
void *kheap_extend()
{
	page_alloc(kheap_top);
	va_t page = kheap_top;
	kheap_top += PAGE_NBYTES;
	return (void *)page;
}