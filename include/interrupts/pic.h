#pragma once

#include <io.h>

#define PIC_MASTER_CMD_PORT  0x20
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_CMD_PORT   0xA0
#define PIC_SLAVE_DATA_PORT  0xA1

void pic_remap_irqs();
void pic_master_reset();
void pic_slave_reset();
