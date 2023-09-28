#include <io.h>

// write a word to the port.
void outb(uint16_t port, uint8_t value)
{
	asm("outb %1, %0" : : "d"(port), "a"(value));
}

// write a word to the port.
// should be 2 byte aligned port number.
void outw(uint16_t port, uint16_t value)
{
	asm("outw %1, %0" : : "dN"(port), "a"(value));
}

// write a double-word to the port.
// should be 4 byte aligned port number.
void outl(uint16_t port, uint32_t value)
{
	asm("outl %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm("inb %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}

uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm("inw %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}

uint32_t inl(uint16_t port)
{
	uint32_t ret;
	asm("inl %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}