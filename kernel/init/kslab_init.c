#include <memory/kslab.h>

static bool init_done = false;
void kcache_init()
{
	if (init_done) {
		return;
	}
	
	kcache_t *cache_cache = get_cache_cache();
	kcache_grow(cache_cache, 1);

	kcache_t **caches = get_cache_arr();
	for (int i = 0; i < KCACHE_NUM; i++) {
		kcache_grow(caches[i], 1);
		kcache_add(caches[i]);
	}
	init_done = true;
}
