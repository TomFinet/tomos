/*
 * We have three types of gates in the interrupt descriptor table:
 *
 * 		1. interrupt: these point to an interrupt handler. interrupts
 * are called asynchronously by external hardware. the processor state is saved
 * so that execution can resume after the interrupt handler has finished
 * executing.
 * 		2. trap: traps are exceptions triggered immediately after
 * executing a trapping instruction. thus traps are synchronous and execution
 * resumes after the trap handler.
 * 		3. task:
 */

#pragma once

#include <interrupts/pic.h>
#include <interrupts/isr_def.h>

#include <klib/kstdio.h>
#include <klib/kstring.h>

#define IDT_GATE_NUM ISR_NUM
#define IDT_IRQ0     32

#define GATE_PRES(x) ((x) << 7)
#define GATE_DPL(x)  ((x) << 5)
#define GATE_SIZE(x) ((x) << 3)

#define TASK_FLAGS      (GATE_PRES(1) | GATE_DPL(0) | GATE_SIZE(1) | 0b101)
#define INTERRUPT_FLAGS (GATE_PRES(1) | GATE_DPL(0) | GATE_SIZE(1) | 0b110)
#define TRAP_FLAGS      (GATE_PRES(1) | GATE_DPL(0) | GATE_SIZE(1) | 0b111)

/// 8-byte aligned for improved cache line fill.
struct idt_gate {
	uint16_t offset_low;
	uint16_t segment_selector;
	uint8_t zero;
	uint8_t flags;
	uint16_t offset_high;
} __attribute__((packed)) __attribute__((aligned(8)));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void idt_init();

extern void idt_set(uint32_t idt_addr);

extern void isr_13(void);
extern void isr_14(void);
