#include <klib/assert.h>

void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
        fprintk("Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
        kpanic();
}
