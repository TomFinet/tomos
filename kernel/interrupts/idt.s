.global idt_set

idt_set:
	mov 4(%esp), %eax
	lidt (%eax)
	ret
