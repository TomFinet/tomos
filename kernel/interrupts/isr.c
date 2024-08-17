#include <interrupts/isr.h>
#include <panic.h>

void print_interrupt_frame(struct interrupt_frame regs)
{
	fprintk("\
		isr_num: %d, err_code: %h\n\
		ds: %h\n\
		eax: %h, ecx: %h, edx: %h\n\
		ebx: %h, old_esp: %h, ebp: %h\n\
		esi: %h, edi: %h\n\
		eip: %h, cs: %h, eflags: %b\n\n",
		regs.isr_num, regs.err_code, regs.ds, regs.eax, regs.ecx,
		regs.edx, regs.ebx, regs.esp_old, regs.ebp, regs.esi, regs.edi,
		regs.eip, regs.cs, regs.eflags);
}

void exception_handler(struct interrupt_frame regs)
{
	printk("exception_handler called...\n");
	print_interrupt_frame(regs);
	kpanic();
	return;
}

void irq_handler(struct interrupt_frame regs)
{
	printk("irq_handler called...\n");
	print_interrupt_frame(regs);
	return;
}