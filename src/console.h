#ifndef CONSOLE_HANDLING_H
#define CONSOLE_HANDLING_H

#include "ktypes.h"
#include "ring_buffer.h"

struct console {
	u8	x;
	u8	y;
	u8	color;
	u16	*screen;
};

void	ft_console(struct console *, struct ring *);
void	init_console(struct console *);
void	move_cursor(struct console *, u8, u8);
void	print_string(struct console *, char const *);
void	print_char(struct console *, char const);
void	screen_clear(struct console *);
void	scroll_console(struct console *);

#endif
