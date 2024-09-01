#pragma once

#define SYSTEM_MEM 0x20000000 /* 512 MiB */

#define KERNEL_PA_BASE 0x00200000 /* 2MB */
#define KERNEL_VA_BASE 0xC0000000 /* 3GB */

#define PAGE_ORDER 12
#define PTE_ORDER 10
#define PDE_ORDER 10

#define KERNEL_PDE_BASE KERNEL_VA_BASE >> 22
#define KERNEL_PTE_BASE (KERNEL_VA_BASE >> PAGE_ORDER)

#define ORDER_SHL(o) (1 << (o))
