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

void kmain() {
    serial_init();
    gdt_init();
    idt_init();
    asm volatile ("int $0x1");
    asm volatile ("int $0x20");
}