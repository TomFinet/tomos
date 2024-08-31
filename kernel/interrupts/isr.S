#include <interrupts/isr_def.h>

.extern isr_master

isr_common:
	pusha

	mov %ds, %ax
	pushl %eax

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call isr_master

	popl %eax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	popa
	add $0x8, %esp	# clear isr_num and error code
	iret

.macro isr_err, isr_num
.global isr_\isr_num
isr_\isr_num:
	pushl $\isr_num
	jmp isr_common
.endm

.macro isr_no_err, isr_num
.global isr_\isr_num
isr_\isr_num:
	pushl $0x0
	pushl $\isr_num
	jmp isr_common
.endm

isr_err ISR_GP
isr_err ISR_PF
