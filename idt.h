#ifndef IDT_H
#define IDT_H

#include "stdint.h"

extern void outb(uint16_t port, uint16_t val); 

// Must be 8 octets
struct InterruptDescriptor32 {
	uint16_t offset_1;		 // offset bits 0..15
	uint16_t selector;		 // a code segment selector in GDT or LDT
	uint8_t zero;			 // unused, set to 0
	uint8_t type_attributes; // gate type, dpl, and p fields
	uint16_t offset_2;		 // offset bits 16..31
} __attribute__((packed)); // dis au compilateur de ne pas ajouter de padding pour faire 8oct

// remplit une entree
void set_idt_gate(uint8_t num, uint32_t base);
// construit les 256 entrees + charge avec lidt (helper asm)
void idt_init();

#endif // !IDT_H
