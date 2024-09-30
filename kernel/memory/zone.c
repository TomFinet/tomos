#include <memory/zone.h>

static zone_t zone_linear = ZONE_LINEAR_INIT;
static zone_t zone_adhoc = ZONE_ADHOC_INIT;

zone_t* get_zone_linear(void)
{
	return &zone_linear;
}

zone_t* get_zone_adhoc(void)
{
	return &zone_adhoc;
}

void zone_mark_pages_used(zone_t* zone, pg_idx_t pg_start, pg_idx_t pg_end)
{
	unsigned long bitmap_start = pg_start - zone->pg_offset;
	unsigned long bitmap_end = pg_end - zone->pg_offset;
	for (unsigned long i = bitmap_start; i < bitmap_end; i++) {
		bitmap_set(zone->free_pages, i);
	}
}
