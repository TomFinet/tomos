#include <startup/gdt.h>

struct gdt_entry gdt_entries[GDT_ENTRY_NUM];

void create_descriptor(uint8_t i, uint32_t base, uint32_t limit, uint16_t flags) {
    gdt_entries[i].limit = limit & 0x0000ffff;
    gdt_entries[i].base_low = base & 0x0000ffff;
    gdt_entries[i].base_mid = base & 0x00ff0000;
    gdt_entries[i].flags = ((limit >> 8) & 0x000f0000)  | (flags & 0xf0ff);
    gdt_entries[i].base_high = base >> 24;
}

void gdt_init() {
    struct gdt_ptr gdtr = {(sizeof(struct gdt_entry) * GDT_ENTRY_NUM) - 1, (uint32_t) &gdt_entries};
    create_descriptor(0, 0, 0, 0); // null segment descriptor
    create_descriptor(1, FLAT_BASE, FLAT_LIMIT, (GDT_CODE_PL0));
    create_descriptor(2, FLAT_BASE, FLAT_LIMIT, (GDT_DATA_PL0));
    create_descriptor(3, FLAT_BASE, FLAT_LIMIT, (GDT_CODE_PL3));
    create_descriptor(4, FLAT_BASE, FLAT_LIMIT, (GDT_DATA_PL3));
    gdt_set((uint32_t) &gdtr);
}


