/**
 * Author:    Tom Finet
 * Created:   22/03/2023
 * 
 * Kernel entry point.
 * 
 **/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <serial.h>

#include <memory/gdt.h>
#include <memory/mm.h>
#include <memory/paging.h>
#include <memory/vmm.h>

#include <interrupts/idt.h>

#include <klib/kstdio.h>

void print_page_table(uint32_t* pg_start) {
    for(uint32_t i = 0; i < 1024; i++) {
        fprintk("%d: %h\n", i, *(pg_start + i));
    }
    printk("\n");
}

void kentry(void) {
    serial_init();
    gdt_init();
    idt_init();
    mm_init();
    page_init();
    vmm_init();
}