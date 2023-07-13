#include <panic.h>

void kpanic(void) {
	printk("Kernel panic!!!");
	for(;;) { }
}