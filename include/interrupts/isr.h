#pragma once

#include <stdint.h>
#include <klib/kstdio.h>

/// view of the stack frame setup by an interrupt
struct interrupt_frame {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp_old, ebx, edx, ecx, eax;
	uint32_t isr_num;
	uint32_t err_code, eip, cs, eflags, esp_user, ss;
};

void exception_handler(struct interrupt_frame regs);
void irq_handler(struct interrupt_frame regs);