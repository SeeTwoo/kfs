#include "interrupts.h"
#include "ktypes.h"

extern void load_idt(void *idtr);
void keyboard_handler_c(void);
extern int g_interrupt;
extern u8 inb(u16);
extern void outb(u16, u16);

static struct InterruptDescriptor32 idt[256];
struct IDTR {
	u16 limit;
	u32 base;
} __attribute__((packed));

// num -> interrupt num (32 for keyboard)
// base -> handler memory address
void set_idt_gate(u8 num, u32 base) {
	idt[num].offset_1 = base & 0xFFFF;
	idt[num].selector = 0x08;
	idt[num].zero = 0;
	idt[num].type_attributes = 0x8E;
	idt[num].offset_2 = (base >> 16) & 0xFFFF;
}

void idt_init() {

	// retrieve size and limit
	struct IDTR idtr;

	idtr.limit = 256 * 8 - 1;
	idtr.base = (u32)idt;
	//call lidt
	load_idt(&idtr);
};

void c_handler(u32 const int_no)
{
	if (int_no == 0x21)
		keyboard_handler_c();   /* défini dans keyboard_handler.c */
	outb(0x20, 0x20);
}
