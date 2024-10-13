#include <memory/memory_def.h>
#include <grub_def.h>

.extern kentry

.section .multiboot
.align 4
.long GRUB_MAGIC
.long GRUB_FLAGS
.long GRUB_CHECKSUM

.section .bss.pagetables
.align PAGE_NBYTES

.global _page_dir
_page_dir:
	.fill KERNEL_PDE_BASE, 4, 0
kernel_page_dir_start:
	.fill (PDE_COUNT - KERNEL_PDE_BASE), 4, 0

.global _kernel_pg_table_0
_kernel_pg_table_0:
	.fill PTE_COUNT, 4, 0

.global _kernel_pg_table_1
_kernel_pg_table_1:
	.fill PTE_COUNT, 4, 0

.global _tmp_pg_table
.global _identity_page_table
_tmp_pg_table:
_identity_page_table:
	.fill PTE_COUNT, 4, 0

.section .text.low
.global _start
.type _start, @function
_start:
	# identity mapping
	movl $_identity_page_table, %eax
	movl $0, %ebx
	call map_page_table

	orl $3, %eax
	movl %eax, _page_dir

	# higher quarter kernel mapping
	movl $_kernel_pg_table_0, %eax
	movl $0, %ebx
	call map_page_table

	orl $3, %eax
	movl %eax, kernel_page_dir_start

	movl $_kernel_pg_table_1, %eax
	movl $(PTE_COUNT * PAGE_NBYTES), %ebx
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
	addl $PAGE_NBYTES, %ebx
	cmp $PTE_COUNT, %ecx
	jb next
	ret

.section .text

kernel_higher:
	movl $stack_top, %esp

	call kentry

	cli
	1: hlt
	jmp 1b


.section .bss 
.align 16

# grows from high to low address
stack_bottom:
.skip KiB(16) # 16384 Bytes
stack_top:
