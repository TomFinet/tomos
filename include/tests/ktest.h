#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <interrupts/idt.h>
#include <interrupts/isr.h>
#include <memory/gdt.h>

#include <klib/kstdio.h>

#define KTEST(name)                                                            \
	{                                                                      \
		#name, name, true                                              \
	}

#define KTEST_SUITE(suite)                                                     \
	static struct ktest_suite_t *__test_suite_##suite                      \
		__attribute__((section(".ktest"))) __attribute__((used)) =     \
			&suite;

// TODO: don't like the global state
extern bool curr_test_passing;
#define ASSERT(b)                                                              \
	if (!(b)) {                                                            \
		fprintk("Assert failed on line %d!\n", __LINE__);              \
		curr_test_passing = false;                                     \
		return;                                                        \
	}

/* all information to execute and track the status of a ktest */
struct ktest_case_t {
	char name[32];
	void (*run)(void);
	bool passed;
};

// groups related tests into the same "suite".
struct ktest_suite_t {
	char name[32];
	void (*init)(void);
	void (*exit)(void);
	struct ktest_case_t *cases;
	uint8_t num_cases;
};

void ktest_init(void);

void ktest_run_all();

void ktest_run_suite(struct ktest_suite_t *suite);
