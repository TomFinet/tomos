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

#include <gdt.h>
#include <serial.h>
#include <pci.h>
#include <bochs_vga.h>
 
void kmain() {
    gdt_init();
    serial_init();
}