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

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <io.h>
#include <klib/kstdio.h>
#include <list.h>

// using: https://wiki.osdev.org/PCI#Configuration_Space_Access_Mechanism_.231
// as main reference.

// config ports
#define PCI_CONFIG_ADDRESS_PORT 	0xCF8
#define PCI_CONFIG_DATA_PORT 		0xCFC

// device structure offsets
#define PCI_VENDOR_ID				0x0
#define PCI_DEV_ID 					0x2
#define PCI_COMMAND 				0x4
#define PCI_STATUS 					0x6
#define PCI_REVISION_ID				0x8
#define PCI_PROG_IF					0x9
#define PCI_SUBCLASS_CODE			0xA
#define PCI_CLASS_CODE				0xB
#define PCI_CACHE_LINE_SIZE			0xC
#define PCI_LATENCY_TIMER			0xD
#define PCI_HEADER_TYPE				0xE
#define PCI_BIST 					0xF
#define PCI_BAR0					0x10
#define PCI_BAR1					0x14
#define PCI_BAR2 					0x18
#define PCI_BAR3 					0x1C
#define PCI_BAR4 					0x20
#define PCI_BAR5 					0x24
#define PCI_CARDBUS_CIS_PTR			0x28
#define PCI_SUBSYSTEM_VENDOR_ID		0x2C
#define PCI_SUBSYSTEM_ID 			0x2E
#define PCI_ROM_BASE_ADDRESS 		0x30
#define PCI_CAPABILITIES_PTR		0x34
#define PCI_INTERRUPT_LINE			0x3C
#define PCI_INTERRUPT_PIN			0x3D
#define PCI_MIN_GRANT				0x3E
#define PCI_MAX_LATENCY				0x3F

#define PCI_SECONDARY_BUS			0x19


// class codes
#define PCI_BRIDGE					0x6

// subclass codes
#define PCI_TO_PCI_BRIDGE			0x4


#define PCI_GENERAL					0x0
#define PCI_TO_PCI					0x1
#define PCI_TO_CARDBUS				0x2
#define PCI_MULTI_FUNCTION   		0x80

#define PCI_NO_DEV 					0xFFFF

#define PCI_DEV_NUMBER				32
#define PCI_FUNCTION_NUMBER 		8

struct pci_id {
	uint16_t vendor;
	uint16_t device;
};

struct pci_class {
	uint8_t subclass;
	uint8_t cls;
};

struct pci_device {
	struct pci_id id; 
	uint8_t bus;
	uint8_t dev;
	uint8_t func;
	struct list_head* pci_list;
};

/// initialise the pci linked-list and enumerate all pci devices.
void pci_init();

uint32_t pci_config(uint8_t bus, 
					uint8_t dev,
					uint8_t func,
					uint8_t offset) ;

void pci_register(struct pci_device* dev);

void pci_enumerate();

void pci_enumerate_bus(uint8_t bus);

void pci_enumerate_device(uint8_t bus, uint8_t dev);

void pci_enumerate_function(uint8_t bus, uint8_t dev, uint8_t func, struct pci_id id);

/// looks for a pci device in the devices linked-list.
struct pci_device* pci_find(struct pci_id id);


bool pci_same_dev(struct pci_id id1, struct pci_id id2);