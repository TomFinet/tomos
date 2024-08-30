#include <interrupts/idt.h>

struct idt_gate gates[IDT_GATE_NUM];
static struct idt_ptr idtr = {sizeof(struct idt_gate) * IDT_GATE_NUM - 1,
			(uint32_t)&gates};

void idt_create_descriptor(uint8_t i, uint32_t offset, uint16_t ss,
			   uint8_t flags)
{
	gates[i].offset_low = offset & 0xffff;
	gates[i].segment_selector = ss;
	gates[i].zero = 0;
	gates[i].flags = flags;
	gates[i].offset_high = offset >> 16;
}

void idt_init()
{
	bzero(&gates, sizeof(struct idt_gate) * IDT_GATE_NUM - 1);
	idt_create_descriptor(13, (uint32_t)isr_13, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(14, (uint32_t)isr_14, 0x08, INTERRUPT_FLAGS);
	idt_set((uint32_t)&idtr);
}
