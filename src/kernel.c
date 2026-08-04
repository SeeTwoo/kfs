#include "console_handling.h"
#include "idt.h"
#include "ring_buffer.h"
#include "shell.h"

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

void kmain(void) {

	pic_remap();
	idt_init();
	init_ring(&kbd_ring);
	set_idt_gate(0x21, (uint32_t)isr21);
	__asm__("sti");
	screen_clear();
	print_string(20, 12, "42", 0x01);
	shell();
	while (1) ;
}
