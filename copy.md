void	keyboard_handler_c(void)
{
	unsigned char	scancode = inb(0x60);

	if (scancode > 9)
		return ;
	char	*vidmem = (char *)0xb8000;
	vidmem[0] = ps2_set[scancode];
	outb(0x20, 0x20);
}



static struct state	awix;
extern u8	ps2_set[];

static inline void	outb(uint16_t port, u8 val)
{
	asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline u8	inb(uint16_t port)
{
	u8	ret;

	asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
	return ret;
}


void	pic_remap()
{
	outb(0x20, 0x11); outb(0xA0, 0x11);
	outb(0x21, 0x20); outb(0xA1, 0x28);
	outb(0x21, 0x04); outb(0xA1, 0x02);
	outb(0x21, 0x01); outb(0xA1, 0x01);
	outb(0x21, 0x0);  outb(0xA1, 0x0);
}

ps2_set.c

#include "ktypes.h"

u8	ps2_set[] = {
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


