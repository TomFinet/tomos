#pragma once

#define ALIGN         0x1
#define MEMINFO       0x2
#define GRUB_FLAGS    ALIGN | MEMINFO
#define GRUB_MAGIC    0x1BADB002
#define GRUB_CHECKSUM -(GRUB_MAGIC + GRUB_FLAGS)
