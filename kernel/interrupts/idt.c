#include <interrupts/idt.h>

struct idt_gate gates[IDT_GATE_NUM];

void idt_create_descriptor(uint8_t i, uint32_t offset, uint16_t ss, uint8_t flags) {
	gates[i].offset_low = offset & 0xffff;
	gates[i].segment_selector = ss;
	gates[i].zero = 0;
	gates[i].flags = flags;
	gates[i].offset_high = offset >> 16;
}

void idt_init() {
	struct idt_ptr idtr = {sizeof(struct idt_gate) * IDT_GATE_NUM - 1, (uint32_t) &gates};
	bzero(&gates, sizeof(struct idt_gate) * IDT_GATE_NUM - 1);

	idt_create_descriptor(0,  (uint32_t) isr0,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(1,  (uint32_t) isr1,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(2,  (uint32_t) isr2,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(3,  (uint32_t) isr3,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(4,  (uint32_t) isr4,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(5,  (uint32_t) isr5,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(6,  (uint32_t) isr6,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(7,  (uint32_t) isr7,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(8,  (uint32_t) isr8,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(9,  (uint32_t) isr9,  0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(10, (uint32_t) isr10, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(11, (uint32_t) isr11, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(12, (uint32_t) isr12, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(13, (uint32_t) isr13, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(14, (uint32_t) isr14, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(15, (uint32_t) isr15, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(16, (uint32_t) isr16, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(17, (uint32_t) isr17, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(18, (uint32_t) isr18, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(19, (uint32_t) isr19, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(20, (uint32_t) isr20, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(21, (uint32_t) isr21, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(22, (uint32_t) isr22, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(23, (uint32_t) isr23, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(24, (uint32_t) isr24, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(25, (uint32_t) isr25, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(26, (uint32_t) isr26, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(27, (uint32_t) isr27, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(28, (uint32_t) isr28, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(29, (uint32_t) isr29, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(30, (uint32_t) isr30, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(31, (uint32_t) isr31, 0x08, INTERRUPT_FLAGS);

	idt_create_descriptor(IDT_IRQ0 + 0, (uint32_t) irq0, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(IDT_IRQ0 + 1, (uint32_t) irq1, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(IDT_IRQ0 + 2, (uint32_t) irq2, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(IDT_IRQ0 + 3, (uint32_t) irq3, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(IDT_IRQ0 + 4, (uint32_t) irq4, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(IDT_IRQ0 + 5, (uint32_t) irq5, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(IDT_IRQ0 + 6, (uint32_t) irq6, 0x08, INTERRUPT_FLAGS);
	idt_create_descriptor(IDT_IRQ0 + 7, (uint32_t) irq7, 0x08, INTERRUPT_FLAGS);
	
	idt_set((uint32_t) &idtr);
}