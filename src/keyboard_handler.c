#include <stdint.h>

#include "ring_buffer.h"

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);
extern struct ring_buffer kbd_ring;

void	keyboard_handler_c(void)
{
	ring_push(&kbd_ring, inb(0x60));
}
