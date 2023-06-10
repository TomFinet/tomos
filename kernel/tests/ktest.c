#include <tests/ktest.h>

// addresses of start and end of linker section
// containing a contiguous block of struct ktest_case_t test cases.
extern uint32_t _ktest_start;
extern uint32_t _ktest_end;

static struct ktest_suite_t* ktest_suite_start;
static struct ktest_suite_t* ktest_suite_end;

static inline void ktest_print_suite_header(struct ktest_suite_t* suite) {
	printk("------------------ Running tests for ");
	printk(suite->name);
	printk(" ------------------\n");
}

static void ktest_print_case_result(struct ktest_case_t* tcase) {
	printk("Running test case ");
	printk(tcase->name);
	printk(":\t");
	if(tcase->passed) {
		printk("passed\n");
	} else {
		printk("failed\n");
	}
}

void ktest_init(void) {
	ktest_suite_start = (struct ktest_suite_t *) &_ktest_start;
	ktest_suite_end = (struct ktest_suite_t *) &_ktest_end;

	fprintk("ktest_suite_end = %h\n", ktest_suite_end);
	fprintk("ktest_suite_start = %h\n", ktest_suite_start);
}

// TODO: when I replace with 
// for(struct ktest_suite_t* s = ktest_suite_start; s < ktest_suite_end; s++)
// it can't read the structure.
// behaviour of the pointers here makes no sense to me...
void ktest_run_all() {
	ktest_run_suite(_ktest_start);
}

void ktest_run_suite(struct ktest_suite_t* suite) {

	ktest_print_suite_header(suite);
	suite->init();

	int pass_count = 0;
	uint32_t case_count = sizeof(*(suite->cases)) / sizeof(struct ktest_case_t);

	for(struct ktest_case_t* t = suite->cases;
		t < suite->cases + case_count; t++) {
		t->run();
		pass_count += t->passed;

		ktest_print_case_result(t);
	}

	fprintk("total tests passed: %d / %d\n\n", pass_count, case_count);

	suite->exit();
}