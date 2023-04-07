#include <gdt.h>

gdt_entry_t gdt_entries[GDT_NUM_ENTRIES];

void gdt_init() {
    gdt_ptr_t gdtr = {sizeof(gdt_entry_t) * GDT_NUM_ENTRIES, (uint32_t) &gdt_entries};
    create_descriptor(0, 0, 0, 0);
    create_descriptor(1, 0, 0x000fffff, (GDT_CODE_PL0));
    create_descriptor(2, 0, 0x000fffff, (GDT_DATA_PL0));
    create_descriptor(3, 0, 0x000fffff, (GDT_CODE_PL3));
    create_descriptor(4, 0, 0x000fffff, (GDT_DATA_PL3));
    gdt_set((uint32_t) &gdtr);
    printk("gdt set\n");
}

void create_descriptor(uint8_t i, uint32_t base, uint32_t limit, uint16_t flags) {
    gdt_entries[i].segment_limit = limit & 0x0000ffff;
    gdt_entries[i].low_base_address = base & 0x0000ffff;
    gdt_entries[i].mid_base_address = base & 0x00ff0000;
    gdt_entries[i].flags = ((limit >> 8) & 0x000f0000)  | (flags & 0xf0ff);
    gdt_entries[i].high_base_address = base >> 24;
}
 
