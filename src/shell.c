#include "console_handling.h"
#include "kernel.h"
#include "ring_buffer.h"

extern struct ring_buffer kbd_ring;
extern struct kernel awix;
extern const u8 ps2_set[];

void	init_awix(struct kernel *awix)
{
	awix->cursor.x = 0;
	awix->cursor.y = 0;
	awix->color = 0x0F;
}

void	kputchar(struct kernel *awix, char const c)
{
	char	*screen = (char *)0xb8000;

	if (awix->cursor.x == 80)
		move_cursor(awix, 0, awix->cursor.y + 1);
	if (awix->cursor.y == 25) {
		scroll_console(awix);
		awix->cursor.y = 24;
	}
	print_char(awix->cursor.x, awix->cursor.y, c, awix->color);
	awix->cursor.x++;
}

void	kputs(struct kernel *awix, char const *s)
{
	for (; *s; s++)
		kputchar(awix, *s);
}

void	new_line(struct kernel *awix)
{
	if (awix->cursor.y == 24) {
		scroll_console(awix);
		move_cursor(awix, 0, 24);
		return ;
	}
	move_cursor(awix, 0, awix->cursor.y + 1);
}

void	shell()
{
	struct kernel	awix;

	init_awix(&awix);
	while (1) {
		asm volatile("cli");
		if (kbd_ring.count == 0)
			asm volatile("sti; hlt");
		asm volatile("sti");
		u8	scancode = ring_pop(&kbd_ring);
		if (scancode & 0x80)
			continue ;
		u8	c = ps2_set[scancode];
		if (!c)
			continue ;
		if (c == '\n')
			new_line(&awix);
		else
			kputchar(&awix, c);
	};
}
