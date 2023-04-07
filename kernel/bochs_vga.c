#include <bochs_vga.h>
#include <serial.h>

struct pci_id bochs_vga_id = {0x1234, 0x1111};

/// physical address of the framebuffer.
uint32_t* framebuffer_paddr;
uint16_t resX;
uint16_t resY;
uint8_t bytePerPixel;

void bochs_vga_init() {
	bochs_vga_configure(VBE_DISPI_MAX_XRES / 2, VBE_DISPI_MAX_YRES / 2);

	serial_write('A');

	// TODO: bug here
	struct pci_device* vga = pci_find(bochs_vga_id);
	if(vga == NULL) {
		// device does not exist.
		return;
	}

	serial_write('B');

	framebuffer_paddr = (uint32_t *) pci_config(
		vga->bus, vga->dev, vga->func, PCI_BAR0);
}

void bochs_vga_configure(uint16_t x, uint16_t y) {
	bochs_vga_write_register(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
	bochs_vga_write_register(VBE_DISPI_INDEX_XRES, x);
	bochs_vga_write_register(VBE_DISPI_INDEX_YRES, y);
	bochs_vga_write_register(VBE_DISPI_INDEX_BPP, VBE_DISPI_BPP_32);
	bochs_vga_write_register(VBE_DISPI_INDEX_ENABLE, 
		VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);

	resX = x;
	resY = y;
	bytePerPixel = 4;
}

void bochs_vga_write_register(uint16_t index, uint16_t data) {
	outw(VBE_DISPI_IOPORT_INDEX, index);
	outw(VBE_DISPI_IOPORT_DATA, data);
}

void bochs_vga_set_pixel(uint16_t x, uint16_t y, uint32_t colour) {
	uint32_t offset = (y * resX + x) * bytePerPixel;
	*(framebuffer_paddr + offset) = colour; 
}

/// TODO: should not be in the VGA device driver class
void bochs_draw_char(uint8_t glyph_idx, uint16_t posX, uint16_t posY) {
	// iterate through character 8x8 glyph and set pixels
	for(uint8_t x = 0; x < 8; x++) {
		for(uint8_t y = 0; y < 8; y++) {
			if((font8x8_basic[glyph_idx][y] >> x) & 0x1) {
				bochs_vga_set_pixel(posX + x, posY + y, FG);
			} else {
				bochs_vga_set_pixel(posX + x, posY + y, BG);
			}
		}
	}
}