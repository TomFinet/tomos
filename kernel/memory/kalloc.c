#include <memory/kalloc.h>

void *kmalloc(size_t nbytes)
{
	struct kcache_t *cachep = kcache_best_fit(nbytes);
	if (cachep != NULL) {
		return kcache_alloc(cachep);
	}
	return NULL;
}

void kfree(void *obj)
{
	struct kslab_t *slab = pg_linear_descriptor((va_t)obj)->slab;
	kcache_free(slab, obj);
}