#include "console.h"
#include "kstdlib.h"
#include "ktypes.h"
#include "ring_buffer.h"
#include "io.h"


void disable_vga_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void enable_vga_cursor(u8 cursor_start, u8 cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_vga_cursor(u8 x, u8 y)
{
	u16 pos = y * SCREEN_WIDTH+ x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (u8)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

void	move_cursor(struct console *csl, u8 x, u8 y)
{
	csl->x = x;
	csl->y = y;
	update_vga_cursor(csl->x,csl->y);
}

void	screen_clear(struct console *csl) {
	for (u32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		csl->screen[i] = (csl->color<< 8) | ' ';
}

void	init_console(struct console *csl)
{
	csl->x = 0;
	csl->y = 0;
	csl->color = 0x0F;
	csl->escaped = 0;
	csl->screen = (u16 *)0xb8000;
	csl->buf.n = 0;
	kmemset(csl->buf.buffer, 0, 16);
	screen_clear(csl);
	enable_vga_cursor(0, 15);
	update_vga_cursor(csl->x, csl->y);
}


void	print_char(struct console *csl, char const c)
{
	csl->screen[(csl->y * SCREEN_WIDTH) + csl->x] = (csl->color << 8) | c;
}

void	new_line(struct console *console)
{
	if (console->y == (SCREEN_HEIGHT - 1)) {
		scroll_console(console);
		move_cursor(console, 0, SCREEN_HEIGHT - 1);
		return ;
	}
	move_cursor(console, 0, console->y + 1);
}

// TODO check safety of that
void print_string(struct console *csl, char const *s) {
	for (; *s; s++) {
		if (*s == '\n')
			new_line(csl);
		else
			print_char(csl, *s);
	}
}

void	line_clear(struct console *csl, u8 y)
{
	for (u8 x = 0; x < SCREEN_WIDTH; x++) {
		csl->screen[(y * SCREEN_WIDTH) + x] = (csl->color << 8) | ' ';
	}
}

void	scroll_console(struct console *csl)
{
	kmemmove((u8 *)csl->screen, (u8 *)csl->screen + (SCREEN_WIDTH * 2), (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2);
	line_clear(csl, SCREEN_HEIGHT - 1);
}

static void	advance_cursor(struct console *csl)
{
	csl->x++;
	update_vga_cursor(csl->x,csl->y);
	if (csl->x == SCREEN_WIDTH)
		return new_line(csl);
}

static void	backspace(struct console *csl)
{
	if (csl->x == 0)
		return ;
	csl->x--;
	update_vga_cursor(csl->x,csl->y);
	csl->screen[(csl->y * SCREEN_WIDTH) + csl->x] = (csl->color << 8) | ' ';
}

static void	regular_console(struct console *csl, struct ring *ft_stdout, char c)
{
	if (c == '\n') {
		new_line(csl);
	} else if (c == '\b') {
		backspace(csl);
	} else {
		print_char(csl, c);
		advance_cursor(csl);
	}
}
///////QUICK AND DIRTY
///

///////very VERY bad vt100 command parsing
static void	do_escape_code(struct console *csl)
{
	char	*buf = csl->buf.buffer;
	u8		argument = 0;

	if (*buf == '[')
		buf++;
	while (kisdigit(*buf)) {
		argument = (argument * 10) + (*buf - '0');
		buf++;
	}
	//MAYBE A TABLE LATER WHO FUCKIN KNOWS ?????
	if (argument == 36)
		csl->color = 0x03;
	else if (argument == 31)
		csl->color = 0x04;
	else if (argument == 32)
		csl->color = 0x02;
	else
		csl->color = 0x0F;


	//cleanup
	kmemset(csl->buf.buffer, 0, 16);
	csl->buf.n = 0;
}

static void	fill_buffer(struct console *csl, struct ring *ft_stdout)
{
	while (ft_stdout->count > 0) {
		char	c = ring_pop(ft_stdout);

		if (!c)
			continue ;
		csl->buf.buffer[csl->buf.n] = c;
		if (kisalpha(c)) {
			csl->escaped = 0;
			do_escape_code(csl);
			return ;
		}
		csl->buf.n++;
	}
}

void	ft_console(struct console *csl, struct ring *ft_stdout)
{
	char	c;

	while (1) {
		if (csl->escaped == 1)
			fill_buffer(csl, ft_stdout);

		if (ft_stdout->count == 0)
			break ;
		c = ring_pop(ft_stdout);
		if (!c)
			continue ;
		else if (c == '\x1b')
			csl->escaped = 1;
		else
			regular_console(csl, ft_stdout, c);
	}
}
