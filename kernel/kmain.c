/**
 * Author:    Tom Finet
 * Created:   22/03/2023
 * 
 * Main kernel entry point.
 * The stack is set up.
 * 
 **/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <memory/gdt.h>
#include <interrupts/idt.h>
#include <serial.h>
#include <klib/kstdio.h>

void print_page_table(uint32_t* pg_start) {
    for(uint32_t i = 0; i < 1024; i++) {
        fprintk("%d: %h\n", i, *(pg_start + i));
    }
}

void kmain() {
    serial_init();

    uint32_t* page_dir = (uint32_t *) 0xc0201000;
    print_page_table(page_dir);

    gdt_init();
    idt_init();
}