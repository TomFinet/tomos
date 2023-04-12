.global idt_set

idt_set:
	mov 4(%esp), %eax
	cli
	lidt (%eax)
	sti
	ret
