#include <memory/paging.h>

#include <klib/kbitmap.h>
#include <klib/kstring.h>

#include <ksymbol.h>

SYMBOL_DEFINE(page_dir, pde_t*);
SYMBOL_DEFINE(startup_kernel_mapped_pages, int);

// TODO: bitmap or free list to manage 2^20 pages? 
BITMAP(page_free_map, PAGE_COUNT);

static inline void page_zero(pte_t* page_table)
{
	memset((void*)page_table, 0, PTE_COUNT * sizeof(pte_t));
}

/* reads the page dir at the specified index
and returns the page table address
if the page dir entry is unmapped,
allocate a new frame for the page table. */
static pte_t* pde_read(int pde_idx)
{
	pte_t* page_table;
	pde_t pde = page_dir[pde_idx];
	if (IS_PRESENT(pde)) {
		page_table = (pte_t*)__va(PAGE_PA(pde));
	} else {
		pa_t table_frame = alloc_frame();
		page_table = (pte_t*)__va(table_frame);
		page_zero(page_table);
		page_dir[pde_idx] = PAGE_STD(table_frame);
	}
	return page_table;
}

static inline void page_tlb_invalid(va_t va)
{
	asm volatile("invlpg %0" : : "m"(va) : "memory");
}

static bool init_done = false;
void page_init(void)
{
	if (init_done) {
		return;
	}

	page_dir = (pde_t*)__va(SYMBOL_READ(_page_dir, pa_t));
	startup_kernel_mapped_pages = SYMBOL_READ(_startup_kernel_mapped_pages, int);
	
	int i = 0;
	for (; i < BITMAP_BLK(startup_kernel_mapped_pages); i++) {
		page_free_map[BITMAP_BLKS(KERNEL_PTE_BASE) + i] = ALL_SET;
	}
	
	int part_blk_idx = BITMAP_POS(startup_kernel_mapped_pages);
	if (part_blk_idx) {
		page_free_map[BITMAP_BLKS(KERNEL_PTE_BASE) + i] = (1 << part_blk_idx) - 1;
	}
	init_done = true;
}

void* page_alloc(enum page_type type)
{
	int bitmap_offset = 0;
	int start_page_num = 0;

	if (type == KERNEL) {
		bitmap_offset = BITMAP_BLKS(KERNEL_PTE_BASE);
		start_page_num = KERNEL_PTE_BASE;
	}

	int res = bitmap_first_clear(page_free_map + bitmap_offset,
			PAGE_COUNT - start_page_num + 1);
	if (res == NOT_FOUND) {
		return NULL;
	}
	
	int free_page_num = start_page_num + res; 
	int pde_idx = free_page_num >> PTE_ORDER;
	int pte_idx = free_page_num & 0x3ff;
	pte_t* page_table = pde_read(pde_idx);

	if (IS_PRESENT(page_table[pte_idx])) {
		return NULL;
	}

	pa_t frame = alloc_frame();
	page_table[pte_idx] = PAGE_STD(frame);
	bitmap_set(page_free_map, free_page_num);

	return (void*)(free_page_num << PAGE_ORDER);
}

void* kpage_alloc()
{
	return page_alloc(KERNEL);
}

int page_free(va_t vbase)
{
	if (!IS_ALIGNED(vbase)) {
		return PAGE_INVALID_BASE;
	}

	int pde_idx = PAGE_DIR_IDX(vbase);
	pde_t pde = page_dir[pde_idx];

	if (!IS_PRESENT(pde)) {
		return PAGE_ALREADY_FREE;
	}

	pte_t* page_table = (pte_t*)__va(PAGE_PA(pde));
	uint16_t pte_idx = PAGE_TABLE_IDX(vbase);
	pte_t pte = page_table[pte_idx];

	if (!IS_PRESENT(pte)) {
		return PAGE_ALREADY_FREE;
	}

	page_table[pte_idx] &= ~PAGE_PRESENT(1);
	page_tlb_invalid(vbase);
	bitmap_clear(page_free_map, (pde_idx << PTE_ORDER) + pte_idx);
	free_frame(PAGE_PA(pte));

	return PAGE_FREE_SUCCESS;
}

pa_t page_table_read(va_t vbase)
{
	if (!IS_ALIGNED(vbase)) {
		return PAGE_INVALID_BASE;
	}

	pde_t pde = page_dir[PAGE_DIR_IDX(vbase)];
	if (!IS_PRESENT(pde)) {
		kpanic();
	}

	pte_t* page_table = (pte_t*)__va(PAGE_PA(pde));
	return PAGE_PA(page_table[PAGE_TABLE_IDX(vbase)]);
}

struct frame_t* page_descriptor(void* va)
{
	pa_t page_frame_addr = __pa((va_t)va);
	return pa_to_frame(page_frame_addr);
}