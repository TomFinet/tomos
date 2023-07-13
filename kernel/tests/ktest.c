#include <tests/ktest.h>

/* addresses of start and end of linker section containing
a contiguous block of struct ktest_case_t test cases. */
extern uint32_t _ktest_start;
extern uint32_t _ktest_end;

struct ktest_suite_t** suite_block_start;
struct ktest_suite_t** suite_block_end;

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
  /* _ktest_start is a ptr to a ptr to the first suite */
  suite_block_start = (struct ktest_suite_t**) &_ktest_start;
  suite_block_end = (struct ktest_suite_t**) &_ktest_end;
}

void ktest_run_all() {
  struct ktest_suite_t** suite = suite_block_start; 
  for(; suite < suite_block_end; suite++) {
    ktest_run_suite(*suite);
  }
}

void ktest_run_suite(struct ktest_suite_t* suite) {
	ktest_print_suite_header(suite);
	suite->init();

	int pass_count = 0;
  struct ktest_case_t* t = suite->cases;
	for(; t < suite->cases + suite->num_cases; t++) {
    curr_test_passing = true;
		t->run();
    t->passed = curr_test_passing;
		pass_count += t->passed;
		ktest_print_case_result(t);
	}
	fprintk("total tests passed: %d / %d\n\n", pass_count, suite->num_cases);
	suite->exit();
}