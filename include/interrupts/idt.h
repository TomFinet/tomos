/**
 * We have three types of gates in the interrupt descriptor table:
 * 
 * 		1. interrupt: these point to an interrupt handler. interrupts are called asynchronously
 * 		   by external hardware. the processor state is saved so that execution can resume after
 * 		   the interrupt handler has finished executing.
 * 		2. trap: traps are exceptions triggered immediately after executing a trapping instruction.
 * 		   thus traps are synchronous and execution resumes after the trap handler.
 * 		3. task: 
 */

#pragma once

#include <klib/kstdio.h>
#include <klib/kstring.h>
#include <interrupts/pic.h>

#define IDT_GATE_NUM 256
#define IDT_IRQ0 32

#define GATE_PRES(x) ((x) << 7)
#define GATE_DPL(x)  ((x) << 5)
#define GATE_SIZE(x) ((x) << 3)

#define TASK_FLAGS      (GATE_PRES(1) | GATE_DPL(0) | GATE_SIZE(1) | 0b101)
#define INTERRUPT_FLAGS	(GATE_PRES(1) | GATE_DPL(0) | GATE_SIZE(1) | 0b110)
#define TRAP_FLAGS      (GATE_PRES(1) | GATE_DPL(0) | GATE_SIZE(1) | 0b111)

/// 8-byte aligned for improved cache line fill.
struct idt_gate {
	uint16_t offset_low;
	uint16_t segment_selector;
	uint8_t  always0;
	uint8_t  flags;
	uint16_t offset_high;
} __attribute__ ((packed))
  __attribute__ ((aligned (8)));


struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));


void idt_init();

extern void idt_set(uint32_t idt_addr);

/// these have a meaning defined in the intel manuals.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();