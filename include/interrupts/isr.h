#pragma once

#include <klib/kstdio.h>

#include <stdint.h>

struct isr_frame {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp_old, ebx, edx, ecx, eax;
	uint32_t isr_num, err_code;
	uint32_t eip, cs, eflags;
};

typedef void (*isr_t)(struct isr_frame*);

void isr_init(void);
void isr_register(int isr_num, isr_t isr);
void isr_print_frame(struct isr_frame* frame);
