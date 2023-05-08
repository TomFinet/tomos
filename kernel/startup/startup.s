.set ALIGN,    0x1
.set MEMINFO,  0x2
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS) 

.set kernel_virtual_start, 0xc0000000
.set kernel_page_dir, kernel_virtual_start >> 22

.extern kmain

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss.pagetables
.align 4096

page_dir_start:
	.fill kernel_page_dir, 4, 0
kernel_page_dir_start:
	.fill (1024 - kernel_page_dir), 4, 0

identity_page_table:
	.fill 1024, 4, 0

kernel_page_table_0:
	.fill 1024, 4, 0
kernel_page_table_1:
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
	movl %eax, page_dir_start

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
	movl $page_dir_start, %eax
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
	movl $0, page_dir_start 
	invlpg 0

	movl $stack_top, %esp
	
	call kmain

	cli
	1: hlt
	jmp 1b


.section .bss 
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

