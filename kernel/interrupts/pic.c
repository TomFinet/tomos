#include <interrupts/pic.h>

void pic_remap_irqs()
{
	outb(PIC_MASTER_CMD_PORT, 0x11);
	outb(PIC_SLAVE_CMD_PORT, 0x11);
	outb(PIC_MASTER_DATA_PORT, 0x20);
	outb(PIC_SLAVE_DATA_PORT, 0x28);
	outb(PIC_MASTER_DATA_PORT, 0x04);
	outb(PIC_SLAVE_DATA_PORT, 0x02);
	outb(PIC_MASTER_DATA_PORT, 0x01);
	outb(PIC_SLAVE_DATA_PORT, 0x01);
	outb(PIC_MASTER_DATA_PORT, 0x0);
	outb(PIC_SLAVE_DATA_PORT, 0x0);
}

void pic_master_reset()
{
	outb(PIC_MASTER_CMD_PORT, 0x20);
}

void pic_slave_reset()
{
	outb(PIC_SLAVE_CMD_PORT, 0x20);
}