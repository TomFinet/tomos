#include <pci.h>

struct pci_device devices;

uint8_t pci_count;

/// initialise the pci linked-list and enumerate all pci devices.
void pci_init() {
	list_head_init(devices.pci_list);
	pci_enumerate();
}

uint32_t pci_config(uint8_t bus, 
					uint8_t dev,
					uint8_t func,
					uint8_t offset) {

	uint32_t config_address = 0x800000000 |
							  ((uint32_t)bus  << 16) | 
							  ((uint32_t)dev  << 11) |
							  ((uint32_t)func << 8)  |
							  ((uint32_t)offset & 0xFC); // 4-byte aligned

	outl(PCI_CONFIG_ADDRESS_PORT, config_address);
	return inl(PCI_CONFIG_DATA_PORT);
}

void pci_register(struct pci_device* dev) {
	pci_count++;
	list_add(devices.pci_list, dev->pci_list);
}

void pci_enumerate() {
	// check the the first bus and device for pci host bridge
	uint8_t header_type = (uint8_t) pci_config(0, 0, 0, PCI_HEADER_TYPE);
	// if last bit is set then it has multiple functions.
	if((header_type & PCI_MULTI_FUNCTION) == 0) {
		// single function
		printk("single function device");
		pci_enumerate_bus(0);
	} else {
		// multiple functions, each for enumerating a bus
		printk("multiple function device");
		for(uint8_t f = 0; f < 8; f++) {
			if(pci_config(0, 0, f, PCI_VENDOR_ID) == PCI_NO_DEV) {
				break;
			}
			pci_enumerate_bus(f);
		}
	}
}

void pci_enumerate_bus(uint8_t bus) {
	for(uint8_t dev = 0; dev < PCI_DEV_NUMBER; dev++) {
		pci_enumerate_device(bus, dev);
	}
}

void pci_enumerate_device(uint8_t bus, uint8_t dev) {
	struct pci_id* id = (struct pci_id *) pci_config(bus, dev, 0, PCI_VENDOR_ID);
	if(id->vendor == PCI_NO_DEV) return;

	fprintk("vendor: %d, device: %d\n", id->vendor, id->device);
	
	pci_enumerate_function(bus, dev, 0, *id);
	uint8_t header_type = (uint8_t) pci_config(bus, dev, 0, PCI_HEADER_TYPE);
	if((header_type & PCI_MULTI_FUNCTION) != 0) {
		// multifunction device
		for(uint8_t f = 1; f < PCI_FUNCTION_NUMBER; f++) {
			id = (struct pci_id *) pci_config(bus, dev, f, PCI_VENDOR_ID);
			if(id->vendor != PCI_NO_DEV) {
				pci_enumerate_function(bus, dev, f, *id);
			}
		}
	}
}

void pci_enumerate_function(uint8_t bus, uint8_t dev, uint8_t func, struct pci_id id) {
	struct pci_class* cls = (struct pci_class *) pci_config(bus, dev, func, PCI_SUBCLASS_CODE);
	if(cls->cls == PCI_BRIDGE && cls->subclass == PCI_TO_PCI_BRIDGE) {
		// get the secondary bus this pci-to-pci bridge connects to
		uint8_t secondary_bus = (uint8_t) pci_config(bus, dev, func, PCI_SECONDARY_BUS);
		pci_enumerate_bus(secondary_bus);
	}

	// store the pci device in a table.
	struct pci_device device = {id, bus, dev, func};
	//fprintk("vendor: %d, device: %d", id.vendor, id.device);
	pci_register(&device);
}

/// looks for a pci device in the devices linked-list.
struct pci_device* pci_find(struct pci_id id) {
	// iterate through linked list.
	// check if the ids match if so return
	struct list_head* head = devices.pci_list;
	struct pci_device* dev = list_entry(head, struct pci_device, pci_list);
	
	if(pci_same_dev(dev->id, id)) {
		return dev;
	}

	struct list_head* pos;
	for(pos = head->next; pos != head; pos = pos->next) {
		dev = list_entry(pos, struct pci_device, pci_list);
		if(pci_same_dev(dev->id, id)) {
			return dev;
		}
	}
	return NULL;
}

bool pci_same_dev(struct pci_id id1, struct pci_id id2) {
	return (id1.vendor == id2.vendor &&
	   		id1.device == id2.device);
}