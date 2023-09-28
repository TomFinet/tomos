#include <list.h>
#include <memory/kheap.h>
#include <memory/kslab.h>
#include <tests/ktest.h>

struct test_t {
	int x;
	int y;
	int z;
};

struct kcache_t cache_test = {
	.slab_free = LIST_HEAD_INIT(cache_test.slab_free),
	.slab_partial = LIST_HEAD_INIT(cache_test.slab_partial),
	.slab_full = LIST_HEAD_INIT(cache_test.slab_full),
	.list = LIST_HEAD_INIT(cache_test.list),
	.objsize = sizeof(struct test_t),
};

void suite_init(void)
{
	mm_init();
	page_init();
	kheap_init();
	kcache_init();
}

void suite_exit(void)
{
}

static void test_kcache_grow(void)
{
	int pagenum = 1;
	struct kslab_t *slab = kcache_grow(&cache_test, pagenum);
	ASSERT(slab->pagenum == pagenum);
	ASSERT(slab->capacity == slab->freenum);

	unsigned int useable_size = slab->capacity * cache_test.objsize +
				    sizeof(*slab) +
				    (slab->capacity - 1) * sizeof(va_t);

	unsigned int extra_object = cache_test.objsize + sizeof(va_t);

	ASSERT(useable_size <= KSLAB_SIZE(slab));
	ASSERT(useable_size + extra_object > KSLAB_SIZE(slab));
	ASSERT(cache_test.slab_free.next == &slab->list);
	ASSERT(cache_test.slab_free.prev == &slab->list);

	unsigned int header_size =
		sizeof(*slab) + (slab->capacity - 1) * sizeof(va_t);

	ASSERT((va_t)slab + header_size == slab->slab_mem);
}

static void test_page_descr_knows_slab(void)
{
	struct kslab_t *slab = kcache_grow(&cache_test, 1);
	struct frame_t *descriptor = page_descriptor(slab);
	ASSERT(descriptor->slab == slab);
	descriptor = page_descriptor(slab + 1);
	ASSERT(descriptor->slab == slab);
}

static void test_kcache_alloc_and_free_obj(void)
{
	struct kslab_t *slab = kcache_grow(&cache_test, 1);

	unsigned int prior_freenum = slab->freenum;
	void *obj = kcache_alloc(&cache_test);
	unsigned int after_freenum = slab->freenum;

	ASSERT(prior_freenum == after_freenum + 1);

	for (int i = 0; i < slab->freenum; i++) {
		ASSERT(slab->free_objs[i] != (va_t)obj);
	}

	prior_freenum = slab->freenum;
	kcache_free(&cache_test, obj);
	after_freenum = slab->freenum;

	ASSERT(prior_freenum == after_freenum - 1);
	ASSERT(slab->free_objs[prior_freenum] == (va_t)obj);
}

static struct ktest_case_t kslab_cases[] = {
	KTEST(test_kcache_grow),
	KTEST(test_page_descr_knows_slab),
	KTEST(test_kcache_alloc_and_free_obj),
};

static struct ktest_suite_t kslab_suite = {
	.name = "kslab_suite",
	.init = suite_init,
	.exit = suite_exit,
	.cases = kslab_cases,
	.num_cases = (sizeof(kslab_cases) / sizeof(struct ktest_case_t)),
};

KTEST_SUITE(kslab_suite);