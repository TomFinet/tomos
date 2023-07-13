#include <tests/ktest.h>

void kentry(void) {
  serial_init();
  gdt_init();
  idt_init();

	ktest_init();
	ktest_run_all();
}