#include <memory/memory_def.h>
#include <grub_def.h>

.extern kentry

.section .multiboot
.align 4
.long GRUB_MAGIC
.long GRUB_FLAGS
.long GRUB_CHECKSUM

.section .data
.global _startup_kernel_mapped_pages
.set _startup_kernel_mapped_pages, 2048

.section .bss.pagetables
.align 4096

.global _page_dir
_page_dir:
	.fill KERNEL_PDE_BASE, 4, 0
kernel_page_dir_start:
	.fill (1024 - KERNEL_PDE_BASE), 4, 0

kernel_page_table_0:
	.fill 1024, 4, 0
kernel_page_table_1:
	.fill 1024, 4, 0

identity_page_table:
	.fill 1024, 4, 0

.section .text.low
.global _start
.type _start, @function
_start:
	# identity mapping
	movl $identity_page_table, %eax
	movl $0, %ebx
	call map_page_table

	orl $3, %eax
	movl %eax, _page_dir

	# higher quarter kernel mapping
	movl $kernel_page_table_0, %eax
	movl $0, %ebx
	call map_page_table

	orl $3, %eax
	movl %eax, kernel_page_dir_start

	movl $kernel_page_table_1, %eax
	movl $(1024 * 4096), %ebx
	call map_page_table

	orl $3, %eax
	movl %eax, kernel_page_dir_start + 4

	# set page directory in cr3
	movl $_page_dir, %eax
	movl %eax, %cr3

	# CR0.PE = 1 already since grub puts us in protected mode 
	# set CR0.PG = 1 to enable paging.	
	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0

	jmp kernel_higher

# maps a page table pointed to by eax, starting from physical address ebx
map_page_table:	
	movl $0, %ecx
	orl $3, %ebx
next:
	movl %ebx, (%eax, %ecx, 4)
	incl %ecx
	addl $4096, %ebx
	cmp $1024, %ecx
	jl next 
	ret

.section .text

kernel_higher:
	# unmap the identity page
	# enable NULL ptr exceptions to cause page faults
	movl $0, _page_dir
	invlpg 0
	# TODO: the identity page table is now usable space,
	# we should reclaim it for something.

	movl $stack_top, %esp
	
	call kentry

	cli
	1: hlt
	jmp 1b


.section .bss 
.align 16

# grows from high to low address
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
