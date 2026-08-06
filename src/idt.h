#ifndef IDT_H
#define IDT_H

#include "ktypes.h"

// Must be 8 octets
struct InterruptDescriptor32 {
	u16 offset_1;		 // offset bits 0..15
	u16 selector;		 // a code segment selector in GDT or LDT
	u8 zero;			 // unused, set to 0
	u8 type_attributes; // gate type, dpl, and p fields
	u16 offset_2;		 // offset bits 16..31
} __attribute__((packed)); // dis au compilateur de ne pas ajouter de padding pour faire 8oct

// remplit une entree
void set_idt_gate(u8 num, u32 base);
// construit les 256 entrees + charge avec lidt (helper asm)
void idt_init();

#endif // !IDT_H
