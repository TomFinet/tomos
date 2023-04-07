/// read and write to the serial port.
/// 8-bits, 0-parity, 1 stop bit.

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <io.h>

#define COM1 	0x3F8
#define COM2 	0x2F8

#define SERIAL_DATA						0x0
#define SERIAL_INTERRUPT_ENABLE 		0x1
#define SERIAL_FIFO						0x2
#define SERIAL_LINE_CONTROL				0x3
#define SERIAL_MODEM_CONTROL			0x4
#define SERIAL_LINE_STATUS				0x5

uint8_t serial_init();
uint8_t serial_is_empty();
void serial_write(char c);