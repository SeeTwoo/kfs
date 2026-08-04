#include <stdint.h>

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);
uint8_t	ps2_set[];

void	keyboard_handler_c(void)
{
	unsigned char	scancode = inb(0x60);
	char	*vidmem = (char *)0xb8000;

	if (scancode > 9) {
		return ;
  }
	vidmem[0] = ps2_set[scancode];
}

uint8_t	ps2_set[] = {
	0,
	0,
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
};

