/*

MIT License

Copyright (c) 2023 Tom Finet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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