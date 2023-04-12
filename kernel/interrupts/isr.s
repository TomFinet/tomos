# exceptions

.extern exception_handler

exception_common:
	pusha

	mov %ds, %ax
	push %eax		# save data segment on the stack

	/**mov $0x10, %ax 	# load kernel data segment
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss**/

	call exception_handler

	pop %eax
	/**mov %ax, %ds 	# restore the data segment
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss**/

	popa

	add $8, %esp
	sti
	iret

.macro exception_err, isr_num
	.global isr\isr_num
	isr\isr_num:
		cli
		pushl $\isr_num
		jmp exception_common
.endm

.macro exception_no_err, isr_num
	.global isr\isr_num
	isr\isr_num:
		cli
		pushl $0
		pushl $\isr_num
		jmp exception_common
.endm

exception_no_err 0 			# divide by zero
exception_no_err 1 			# debug exception
exception_no_err 2 			# nmi interrupt
exception_no_err 3 			# breakpoint
exception_no_err 4 			# overflow
exception_no_err 5 			# bound range exceeded
exception_no_err 6 			# invalid opcode
exception_no_err 7 			# device not available (no math coprocessor)
exception_err 8 			# double fault
exception_no_err 9 			# coprocessor segment overrun
exception_err 10 			# invalid tss
exception_err 11 			# segment not present
exception_err 12			# stack-segment fault
exception_err 13			# general protection
exception_err 14 			# page fault
exception_no_err 15 		# intel reserved
exception_no_err 16			# x87 fpu floating-point error
exception_err 17			# alignment check
exception_no_err 18			# machine check
exception_no_err 19			# simd floating-point exception
exception_no_err 20			# virtualization exception
exception_err 21			# control protection exception
exception_no_err 22
exception_no_err 23
exception_no_err 24
exception_no_err 25
exception_no_err 26			# 22-31 intel reserved
exception_no_err 27
exception_no_err 28
exception_no_err 29
exception_no_err 30
exception_no_err 31

# interrupts

/**.extern irq_handler

irq_common:
	pusha

	mov %ds, %ax
	push %eax

	mov $0x10, %ax 	# load kernel data segment
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	call irq_handler

	pop %eax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	popa

	add $8, %esp
	sti
	iret

.macro irq, irq_num
	.global irq\irq_num
	irq\irq_num:
		cli
		pushl $0
		pushl $(\irq_num + 32)
		jmp irq_common
.endm

irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
*/
