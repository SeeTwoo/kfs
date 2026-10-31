#include "events.h"
#include "io.h"
#include "ring_buffer.h"

extern struct ring kbd_ring;
extern const enum events	ps2_set[];
extern const enum events	extended_ps2_set[];

void	keyboard_handler_c(void)
{
	ring_push(&kbd_ring, inb(0x60));
}

void	ft_atkbd(struct ring *events, u8 *multibyte)
{
	while (kbd_ring.count > 0) {
		u8	scancode = ring_pop(&kbd_ring);
		u8	is_release = scancode & 0x80;
		u8	index = scancode & 0x7F;

		if (scancode == 0xE0) {
			*multibyte = 1;
		} else if (*multibyte) {
			ring_push(events, (u8)(extended_ps2_set[index] | is_release));
			*multibyte = 0;
		} else {
			ring_push(events, ((u8)ps2_set[index] | is_release));
		}
	}
}
