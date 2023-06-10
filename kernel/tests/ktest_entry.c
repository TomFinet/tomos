#include <tests/ktest.h>

void kentry(void) {
	ktest_init();
	ktest_run_all();
}