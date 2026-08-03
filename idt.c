#include "idt.h"

extern void load_idt(void *idtr);

static struct InterruptDescriptor32 idt[256];
struct IDTR {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

// num -> interrupt num (32 for keyboard)
// base -> handler memory address
void set_idt_gate(uint8_t num, uint32_t base) {
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
	idtr.base = (uint32_t)idt;
	//call lidt
	load_idt(&idtr);
};
