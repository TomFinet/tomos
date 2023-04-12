# initialise and use the gdt.
#
# flat model, i.e. no segmentation.
# only set up essential segments and make them 
# all over-lap i.e. we only have one
# linear address space.
#
# we have 6 segment descriptors:
# 	- null segment
#	- kernel code
# 	- kernel data
# 	- user code
# 	- user data
# 	- task state (TODO when we introduce context-switch)
# each segment descriptor entry is 64-bits
# each segment will have base linear address 0
# and limit 0xfffff
# for the full details see intel manuals page 3095

.global gdt_set

# tells the cpu the location of gdt table in
# the linear address space.
# takes as parameter the base and limit for the gdtr
gdt_set:
	movl 4(%esp), %eax
	cli
	lgdt (%eax)

	# whatever changes made to the gdt has no effect on
	# the cpu until you load new segment selectors into
	# segment registers.
	mov $0x10, %ax 	# 0x10 is the offset into the data segment
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp $0x08,$.flush # 0x08 is the offset into the code segment of the gdt
.flush:
	sti
	ret

