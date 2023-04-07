// generic I/O functions specific to the x86 ISA
#pragma once

#include <stdint.h>

// write a word to the port.
void outb(uint16_t port, uint8_t value);

// write a word to the port.
// should be 2 byte aligned port number.
void outw(uint16_t port, uint16_t value);

// write a double-word to the port.
// should be 4 byte aligned port number.
void outl(uint16_t port, uint32_t value);

uint8_t inb(uint16_t port);

uint16_t inw(uint16_t port);

uint32_t inl(uint16_t port);