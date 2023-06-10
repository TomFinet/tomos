#pragma once

#include <stdbool.h>

#include <klib/kstdio.h>

#define KTEST(name) { #name, name, true }

#define KTEST_SUITE(suite) \
	static struct ktest_suite_t* __test_suite_##suite \
	__attribute__((section(".ktest"))) \
	__attribute__((used)) \
	__attribute__((aligned(sizeof(struct ktest_suite_t *)))) = &suite;
	

#define ASSERT(b) \
	if(!(b)) { \
		fprintk("Assert failed on line %d!\n", __LINE__); \
		return; \
	}

// defines all information to execute and track the status of a ktest
struct ktest_case_t {
	char name[32];
	void (*run)(void); // the actual test
	bool passed;
};

// groups related tests into the same "suite".
struct ktest_suite_t {
	char name[32];
	void (*init)(void);
	void (*exit)(void);
	struct ktest_case_t* cases;
};

void ktest_init(void);

void ktest_run_all();

void ktest_run_suite(struct ktest_suite_t* suite);
