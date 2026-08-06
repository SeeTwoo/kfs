#include "console.h"
#include "inline_asm.h"
#include "io.h"
#include "kernel.h"
#include "kstdlib.h"
#include "panic.h"
#include "ring_buffer.h"

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
