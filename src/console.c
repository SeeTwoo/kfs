#include "console.h"
#include "kstdlib.h"
#include "ktypes.h"
#include "ring_buffer.h"

void	init_console(struct console *c)
{
	c->x = 0;
	c->y = 0;
	c->color = 0x0F;
	c->screen = (u8 *)0xb8000;
}

void	move_cursor(struct console *c, u8 x, u8 y)
{
	c->x = x;
	c->y = y;
}

void	print_char(struct console *c, char const character)
{
	c->screen[((c->y * 80) + c->x) * 2] = character;
	c->screen[((c->y * 80) + c->x) * 2 + 1] = c->color;
}

void	new_line(struct console *console)
{
	if (console->y == 24) {
		scroll_console(console);
		move_cursor(console, 0, 24);
		return ;
	}
	move_cursor(console, 0, console->y + 1);
}

//TODO check safety of that
void print_string(struct console *c, char const *s) {
	for (; *s; s++) {
		if (*s == '\n')
			new_line(c);
		else
			print_char(c, *s);
	}
}

void	line_clear(struct console *c, u8 y)
{
	for (u8 x = 0; x < 80; x++) {
		c->screen[((y * 80) + x) * 2] = ' ';
		c->screen[((y * 80) + x) * 2 + 1] = 0x0F;
	}
}

void	scroll_console(struct console *c)
{
	kmemmove(c->screen, c->screen + 160, 24 * 80 * 2);
	line_clear(c, 24);
}

void	screen_clear(struct console *c) {
	for (int i = 0; i < 80 * 25; i++) {
		c->screen[i * 2] = ' ';
		c->screen[i * 2 + 1] = 0x0F;
	}
}

void	advance_cursor(struct console *c)
{
	if (c->x == 80)
		return new_line(c);
	c->x++;
}

void	backspace(struct console *c)
{
	if (c->x == 0)
		return ;
	c->x--;
	c->screen[(c->y * 80 * 2) + c->x * 2] = ' ';
}

void	ft_console(struct console *c, struct ring *ft_stdout)
{
	while (ft_stdout->count > 0) {
		u8	character = ring_pop(ft_stdout);

		if (!c) {
			continue ;
		} else if (character == '\n') {
			new_line(c);
		} else if (character == '\b') {
			backspace(c);
		} else {
			print_char(c, character);
			advance_cursor(c);
		}
	}
}
