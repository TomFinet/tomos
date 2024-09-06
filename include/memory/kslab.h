#pragma once

#include <memory/paging.h>
#include <klib/kstring.h>
#include <list.h>

#define KCACHE_NUM 10

#define KSLAB_SIZE(slabp) (slabp->pagenum * PAGE_NBYTES)

#define KSLAB_CAPACITY(slabp, objsize)                                         \
	(KSLAB_SIZE(slabp) - sizeof(struct kslab_t) + sizeof(va_t)) /          \
		(objsize + sizeof(va_t))

#define KSLAB_HEADER_SIZE(slabp, objsize)                                      \
	(sizeof(struct kslab_t) +                                              \
	 (KSLAB_CAPACITY(slabp, objsize) - 1) * sizeof(va_t))

#define KSLAB_AVAIL_BYTES(slabp) (KSLAB_SIZE(slabp) - KSLAB_HEADER_SIZE(slabp))

#define KCACHE_CREATE(osize)                                                   \
	{                                                                      \
		.slab_free = LIST_HEAD_INIT(cache_##osize.slab_free),          \
		.slab_partial = LIST_HEAD_INIT(cache_##osize.slab_partial),    \
		.slab_full = LIST_HEAD_INIT(cache_##osize.slab_full),          \
		.list = LIST_HEAD_INIT(cache_##osize.list),                    \
		.objsize = osize,                                              \
	};

struct kcache_t {
	/* stores pages of free objects. */
	struct list_head slab_free;
	/* stores pages of partially free objects. */
	struct list_head slab_partial;
	/* stores pages of fully allocated objects. */
	struct list_head slab_full;

	struct list_head list;

	/* object size stored in each slab. */
	unsigned int objsize;

	/* constructor and destructor for slab objects. */
	// void (*ctor)();
	// void (*dtor)();
};

/* the slab descriptor is stored on the first page
allocated to the slab. */
struct kslab_t {
	/* the slab list this slab belongs to. */
	struct list_head list;
	struct kcache_t *belongs_to;
	/* the start address of the contiguous number of pages. */
	va_t slab_mem;
	/* the number of contiguous pages in the slab. */
	unsigned int pagenum;
	unsigned int capacity;
	unsigned int freenum;
	/* a list of free slab entry addresses. */
	va_t free_objs[];
};

/* Initialises the kernel slab memory allocator. */
void kcache_init();

/* Adds a cache to the cache list. */
void kcache_add(struct kcache_t *cache);

void *kcache_alloc(struct kcache_t *cache);

void kcache_free(struct kslab_t *slab, void *obj);

/* grow the slab by some number of pages. */
struct kslab_t *kcache_grow(struct kcache_t *cache, unsigned int pagenum);

struct kcache_t *kcache_best_fit(size_t objsize);