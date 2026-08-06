#include "console.h"
#include "inline_asm.h"
#include "io.h"
#include "kernel.h"
#include "panic.h"
#include "ring_buffer.h"

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

void	shutdown()
{
	cli();
	outw(0x604, 0x2000);
	hlt();
	panic();
}

void	shell(struct kernel *awix, struct ring *ft_stdin)
{
	if (ft_stdin->count == 0)
		return ;
	u8	c = ring_pop(ft_stdin);

	if (!c)
		return ;
	else if (c == '\n')
		new_line(awix);
	else if (c == 127)
		shutdown();
	else
		kputchar(awix, c);
}
