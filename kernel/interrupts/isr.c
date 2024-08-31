#include <interrupts/isr_def.h>
#include <interrupts/isr.h>
#include <panic.h>

static isr_t _isr[ISR_NUM];

// TODO: isr should not know internals of PF handler
// move to different file in memory dir
static void page_fault_handler(struct isr_frame* frame)
{
	isr_print_frame(frame);
	kpanic();
}

void isr_print_frame(struct isr_frame* frame)
{
	fprintk("\nisr_num: %d, error code: %d\n"
		"old ds: %h, old cs: %h\n"
		"eax: %h, ecx: %h, edx: %h\n"
		"ebx: %h, old_esp: %h, ebp: %h\n"
		"esi: %h, edi: %h\n"
		"eip: %h\n"
		"eflags: %b\n\n",
		frame->isr_num, frame->err_code,
		frame->ds, frame->cs,
		frame->eax, frame->ecx, frame->edx,
		frame->ebx, frame->esp_old, frame->ebp,
		frame->esi, frame->edi,
		frame->eip,
		frame->eflags);
}

void isr_master(struct isr_frame frame)
{
	_isr[frame.isr_num](&frame);
}

void isr_register(int isr_num, isr_t isr)
{
	_isr[isr_num] = isr;
}

void isr_init(void)
{
	isr_register(ISR_PF, page_fault_handler);
}
