#include "idt.h"

extern void isr21(void);

void	pic_remap()
{
  /*
	outb(0x20, 0x11); outb(0xA0, 0x11);
	outb(0x21, 0x20); outb(0xA1, 0x28);
	outb(0x21, 0x04); outb(0xA1, 0x02);
	outb(0x21, 0x01); outb(0xA1, 0x01);
	outb(0x21, 0x0);  outb(0xA1, 0x0);
  */

outb(0x20, 0x11); outb(0xA0, 0x11);
outb(0x21, 0x20); outb(0xA1, 0x28);
outb(0x21, 0x04); outb(0xA1, 0x02);
outb(0x21, 0x01); outb(0xA1, 0x01);
outb(0x21, 0xFD); outb(0xA1, 0xFF);
}

void screen_clear(char *vidmem) {
	for (int i = 0; i < 80 * 25; i++) {
		vidmem[i * 2] = ' ';
		vidmem[i * 2 + 1] = 0x0F;
	}
}

void print_string(int x, int y, char const *s, char *vidmem) {
	for (; *s; s++) {
		vidmem[((y * 80) + x) * 2] = *s;
		vidmem[((y * 80) + x) * 2 + 1] = 0x01;
		x++;
	}
}

void kmain(void) {
	char *vidmem = (char *)0xb8000;

	// Handle interrupt
  pic_remap();
	idt_init();
	set_idt_gate(0x21, (uint32_t)isr21);
   __asm__("sti");

	screen_clear(vidmem);
	print_string(20, 12, "42", vidmem);
	while (1) ;
}
