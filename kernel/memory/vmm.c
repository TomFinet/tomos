#include <memory/vmm.h>

void *kmalloc(size_t nbytes)
{
	struct kcache_t *cachep = kcache_best_fit(nbytes);
	if (cachep != NULL) {
		return kcache_alloc(cachep);
	}
	return NULL;
}

int kfree(void *objp)
{
	return 0;
}