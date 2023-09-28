TODO:
-----

- add clang-tidy to the project in order to define a coding style.

- finish v1 of uniprocessor memory management system.

- right now, when a page is requested, it is mapped into the
  page table with an associated physical frame of memory.
  We may prefer to map the page, but wait to allocate the
  physical frame until the page is read or written to,
  i.e. demand paging.

- synchronisation primitives.

- NULL is defined as 0, so we need to unmap the first
  0-4KiB of virtual memory so that a NULL reference 
  causes a page table exception.

- parse multiboot info passed in by %ebx

- tests should be self-contained and clean up after themselves.
  perhaps per-test config and cleanup functions are useful.