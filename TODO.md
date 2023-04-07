Roadmap:

- interrupts, and synchronisation setup
	- What are maskable interrupts?
- paging and memory management (malloc and free)
- proper console


Memory management:
------------------

We use a flat memory model with paging for the following reasons:

	- simpler: each process in same linear address space, but linear addresses can be mapped to any physical address, as opposed to 	giving each process its own linear address space.
	- more portable to isa that do not use segmentation.

Read about setting up the gdt for flat memory model.
Read about paging in x86.
Figure out the physical memory map of our system.
Figure out the virtual memory map of our system.

Console:
--------

We need to enumerate the PCI buses and try to detect a Bochs vga device.

We basically need a printk up and running asap.

Then get interrupts working so that we can detect and print system exceptions and faults.
