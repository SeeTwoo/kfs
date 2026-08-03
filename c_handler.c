#include <stdint.h>

void keyboard_handler_c(void);
extern int g_interrupt;
extern uint8_t inb(uint16_t);
extern void outb(uint16_t, uint16_t);

void c_handler(uint32_t int_no)
{
	if (int_no == 0x21){
		keyboard_handler_c();   /* défini dans keyboard_handler.c */

  }
  outb(0x20, 0x20);
}
