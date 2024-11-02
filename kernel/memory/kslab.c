/* For general allocations, the cache is chosen as the smallest slab entries
that are larger than the requested allocation size.

for specific objects that have registered, they have access to their own
cache defined at compile time, so can provide it when allocating and freeing.

the slab that an object belongs to is found by reading the pointer to the slab
embedded at the start of every slab page. kcache_grow is responsible for
embedding this pointer. */

#include <memory/kslab.h>

static kcache_t cache_cache = {
	.slab_free = LIST_HEAD_INIT(cache_cache.slab_free),
	.slab_partial = LIST_HEAD_INIT(cache_cache.slab_partial),
	.slab_full = LIST_HEAD_INIT(cache_cache.slab_full),
	.list = LIST_HEAD_INIT(cache_cache.list),
	.objsize = sizeof(kcache_t),
};

static kcache_t cache_2048 = KCACHE_CREATE(2048);
static kcache_t cache_1024 = KCACHE_CREATE(1024);
static kcache_t cache_512 = KCACHE_CREATE(512);
static kcache_t cache_256 = KCACHE_CREATE(256);
static kcache_t cache_128 = KCACHE_CREATE(128);
static kcache_t cache_64 = KCACHE_CREATE(64);
static kcache_t cache_32 = KCACHE_CREATE(32);
static kcache_t cache_16 = KCACHE_CREATE(16);
static kcache_t cache_8 = KCACHE_CREATE(8);
static kcache_t cache_4 = KCACHE_CREATE(4);

/* caches used by kmalloc for unregistered objects. */
static kcache_t *cache_size[KCACHE_NUM] = {
	&cache_4,   &cache_8,   &cache_16,  &cache_32,   &cache_64,
	&cache_128, &cache_256, &cache_512, &cache_1024, &cache_2048,
};

kcache_t *get_cache_cache(void)
{
	return &cache_cache;
}

kcache_t **get_cache_arr(void)
{
	return cache_size;
}

void kcache_add(kcache_t *cache)
{
	list_add(&cache_cache.list, &cache->list);
}

kslab_t *kcache_grow(kcache_t *cache, unsigned int pagenum)
{
	if (pagenum == 0) {
		return NULL;
	}
	
	va_t cache_va = PAGE_VA(alloc_linear());
	kslab_t *slab_free = (kslab_t *)cache_va;

	slab_free->pagenum = pagenum;
	slab_free->capacity = KSLAB_CAPACITY(slab_free, cache->objsize);
	slab_free->freenum = slab_free->capacity;
	slab_free->slab_mem =
		(va_t)slab_free + KSLAB_HEADER_SIZE(slab_free, cache->objsize);

	slab_free->belongs_to = cache;
	slab_free->list.next = &slab_free->list;
	slab_free->list.prev = &slab_free->list;
	list_add(&cache->slab_free, &slab_free->list);

	for (int i = 0; i < slab_free->freenum; i++) {
		slab_free->free_objs[i] =
			slab_free->slab_mem + i * cache->objsize;
	}

	frame_t *frame = pg_linear_descriptor((va_t)slab_free);
	frame->slab = slab_free;

	for (int i = 1; i < pagenum; i++) {
		va_t slab_va = PAGE_VA(alloc_linear());
		frame = pg_linear_descriptor(slab_va);
		frame->slab = slab_free;
	}

	return slab_free;
}

void *kcache_alloc(kcache_t *cache)
{
	kslab_t *slab;

	if (!list_empty(&cache->slab_partial)) {
		slab = list_entry(cache->slab_partial.next, kslab_t,
				  list);
	} else if (!list_empty(&cache->slab_free)) {
		slab = list_entry(cache->slab_free.next, kslab_t, list);
	} else {
		slab = kcache_grow(
			cache,
			1); /* TODO: is adding one page at a time best? */
		if (slab == NULL) {
			return NULL;
		}
	}

	slab->freenum--;
	void *obj = (void *)slab->free_objs[slab->freenum];

	if (slab->freenum == 0) {
		/* slab is full, move to full list. */
		list_remove(&slab->list);
		list_add(&cache->slab_full, &slab->list);
	} else if (slab->freenum == slab->capacity - 1) {
		/* slab was free but now is partial. */
		list_remove(&slab->list);
		list_add(&cache->slab_partial, &slab->list);
	}

	return obj;
}

void kcache_free(kslab_t *slab, void *obj)
{
	kcache_t *cache = slab->belongs_to;
	slab->free_objs[slab->freenum] = (va_t)obj;
	slab->freenum++;

	/* slabs we are freeing on are by def in slab_partial
	or slab_full, we may need to update the list. */
	if (slab->freenum == slab->capacity) {
		list_remove(&slab->list);
		list_add(&cache->slab_free, &slab->list);
	} else if (slab->freenum == 1) {
		list_remove(&slab->list);
		list_add(&cache->slab_partial, &slab->list);
	}
}

kcache_t *kcache_best_fit(size_t objsize)
{
	unsigned int size = 4;
	for (int i = 1; i <= KCACHE_NUM; i++) {
		if (size >= objsize) {
			return cache_size[i - 1];
		}
		size <<= 1;
	}
	return NULL;
}