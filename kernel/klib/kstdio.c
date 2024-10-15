#include <klib/kstdio.h>

void printk(const char *str)
{
	while (*str != '\0') {
		serial_write(*str);
		str++;
	}
}

/// formatted printk
void fprintk(const char *str, ...)
{
	va_list ap;
	va_start(ap, str);
	vprintk(str, ap);
	va_end(ap);
}

/// outputs a formatted string to serial port.
void vprintk(const char *str, va_list list)
{
	while (*str != '\0') {
		if (*str != '%') {
			serial_write(*str);
			str++;
			continue;
		}
		str++;

		char buffer[256];
		bzero(&buffer, sizeof(buffer));
		char *c;

		if(*str == 's') {
			char *arg = va_arg(list, char *);
			int i = 0;
			while (*arg != '\0') {
				buffer[i] = *arg;
				i++;
				arg++;
			}
			c = buffer;
		}
		else {
			uint32_t arg = va_arg(list, uint32_t);
			if (*str == 'd') {
				c = itoa(arg, buffer, 10);
			} else if (*str == 'b') {
				c = itoa(arg, buffer, 2);
			} else if (*str == 'h') {
				c = itoa(arg, buffer, 16);
			} else {
				c = &buffer[arg];
			}
		}
		printk(c);
		str++;
	}
}

/// doesn't belong in kstdio.c
/// could be optimised for different bases, but oh well...
char *itoa(uint32_t n, char *str, uint8_t base)
{
	int32_t i = -1;
	uint32_t tmp = n;

	while (tmp > 0) {
		i++;
		tmp /= base;
	}

	*(str + i + 1) = '\0';
	while (n > 0) {
		char r = n % base + '0';
		if (r > '9') {
			r += 'A' - '9' - 1;
		}
		*(str + i) = r;
		n /= base;
		i--;
	}

	return str;
}