#include <serial.h>

bool serial_inited = false;

uint8_t serial_init() {
	outb(COM1 + SERIAL_INTERRUPT_ENABLE, 0x00);
	outb(COM1 + SERIAL_LINE_CONTROL, 0x80);  // DLAB to 1
	outb(COM1 + SERIAL_DATA, 0x03);
	outb(COM1 + SERIAL_LINE_CONTROL, 0x00);  // DLAB to 0
	outb(COM1 + SERIAL_LINE_CONTROL, 0x03);  // 8-bit characters, no parity, 1 stop bit
	outb(COM1 + SERIAL_FIFO, 0xC7);          // Enable FIFO, clear them, with 14-byte threshold
	outb(COM1 + SERIAL_MODEM_CONTROL, 0x0B); // IRQs enabled, RTS/DSR set
	outb(COM1 + SERIAL_MODEM_CONTROL, 0x1E); // Set in loopback mode, test the serial chip
	outb(COM1 + SERIAL_DATA, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

	// Check if serial is faulty (i.e: not same byte as sent)
	if(inb(COM1 + SERIAL_DATA) != 0xAE) {
		return 1;
	}

	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	outb(COM1 + SERIAL_MODEM_CONTROL, 0x0F);
	serial_inited = true;
	return 0;
}

uint8_t serial_is_empty() {
	return inb(COM1 + SERIAL_LINE_STATUS) & 0x20;
}

void serial_write(char c) {
	if(!serial_inited) {
		serial_init();
	}
	while (serial_is_empty() == 0);
	outb(COM1, c);
}
