/**
 * Higher-quarter kernel.
 * Virtual address base is 0xc0100000
 * load address is 1MiB = 0x00100000 because grub exists below there.
 */

ENTRY(_start)

_higher_start = 0xc0000000;

SECTIONS
{
	. = 2M;
	_kernel_physical_start = .;

	.early : ALIGN(4K) {
		*(.multiboot)
		*(.text.low)
	}

	.bss.pagetables : ALIGN(4K) {
		*(.bss.pagatables)
	}

	. += _higher_start;
	_kernel_virtual_start = .;

	.text ALIGN(4K) : AT(ADDR(.text) - _higher_start) {
		*(.text)
	}
 
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - _higher_start) { 
		*(.rodata)
	}
 
	.data ALIGN(4K) : AT(ADDR(.data) - _higher_start) {
		*(.data)
	}
 
	.bss ALIGN(4K) : AT(ADDR(.bss) - _higher_start) {
		*(COMMON)
		*(.bss)
	}

	_kernel_virtual_end = .;
}
