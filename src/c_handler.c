#include "ktypes.h"

void keyboard_handler_c(void);
extern int g_interrupt;
extern u8 inb(u16);
extern void outb(u16, u16);

void c_handler(u32 int_no)
{
	if (int_no == 0x21)
		keyboard_handler_c();   /* défini dans keyboard_handler.c */
	outb(0x20, 0x20);
}
