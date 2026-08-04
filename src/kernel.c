#include "idt.h"
#include "kernel.h"
#include "ring_buffer.h"

extern void isr21(void);
extern void	halt();
extern u8 ps2_set[];
extern u8 shift_ps2_set[];

struct ring_buffer kbd_ring;

void	pic_remap()
{
	outb(0x20, 0x11); outb(0xA0, 0x11);
	outb(0x21, 0x20); outb(0xA1, 0x28);
	outb(0x21, 0x04); outb(0xA1, 0x02);
	outb(0x21, 0x01); outb(0xA1, 0x01);
	outb(0x21, 0xFD); outb(0xA1, 0xFF);
}

void screen_clear() {
	char	*vidmem = (char *)0xb8000;

	for (int i = 0; i < 80 * 25; i++) {
		vidmem[i * 2] = ' ';
		vidmem[i * 2 + 1] = 0x0F;
	}
}

void	init_awix(struct kernel *awix)
{
	awix->cursor.x = 0;
	awix->cursor.y = 0;
	awix->color = 0x0F;
}

void	print_char(u8 x, u8 y, char c, u8 color)
{
	char	*vidmem = (char *)0xb8000;

	vidmem[((y * 80) + x) * 2] = c;
	vidmem[((y * 80) + x) * 2 + 1] = color;
}

void print_string(int x, int y, char const *s) {
	char	*vidmem = (char *)0xb8000;

	for (; *s; s++) {
		vidmem[((y * 80) + x) * 2] = *s;
		vidmem[((y * 80) + x) * 2 + 1] = 0x01;
		x++;
	}
}

void	shell()
{
	struct kernel	awix;

	init_awix(&awix);
	while (1) {
		asm volatile("cli");
		if (kbd_ring.count == 0)
			asm volatile("sti; hlt");
		asm volatile("sti");
		u8	scancode = ring_pop(&kbd_ring);
		if (scancode & 0x80)
			continue ;
		u8	c = ps2_set[scancode];
		if (!c)
			continue ;
		print_char(awix.cursor.x, awix.cursor.y, c, awix.color);
		awix.cursor.x++;
	};
}

void kmain(void) {

	pic_remap();
	idt_init();
	init_ring(&kbd_ring);
	set_idt_gate(0x21, (uint32_t)isr21);
	__asm__("sti");
	screen_clear();
	print_string(20, 12, "42");
	shell();
	while (1) ;
}
