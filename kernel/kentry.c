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

void kentry(void)
{
	serial_init();
	gdt_init();
	idt_init();
	mm_init();
	page_init();
}