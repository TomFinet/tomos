#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <serial.h>

#include <memory/gdt.h>
#include <memory/frame.h>
#include <memory/paging.h>
#include <memory/kslab.h>

#include <interrupts/idt.h>

#include <klib/kstdio.h>

void kentry(void)
{
	serial_init();
	gdt_init();
	idt_init();
	frame_init();
	page_init();
	kcache_init();
}