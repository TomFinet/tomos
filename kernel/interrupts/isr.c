#include <interrupts/isr.h>

void print_interrupt_stack(struct interrupt_frame regs) {
	fprintk("isr_num: %h, err_code: %h\n", regs.isr_num, regs.err_code);
	fprintk("eax: %h, ecx: %h, edx: %h\nebx: %h, old_esp: %h, ebp: %h\nesi: %h, edi: %h\n",
			regs.eax, regs.ecx, regs.edx, regs.ebx, regs.esp_old, regs.ebp, regs.esi, regs.edi);
	fprintk("eip: %h, cs: %h, eflags: %b\n\n", regs.eip, regs.cs, regs.eflags);
}

void exception_handler(struct interrupt_frame regs) {
	printk("------- exception_handler called -------\n");
	print_interrupt_stack(regs);
}

void irq_handler(struct interrupt_frame regs) {
	printk("------- irq_handler called -------\n");
	print_interrupt_stack(regs);
}