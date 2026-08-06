#include "kernel.h"
#include "kstdlib.h"
#include "ktypes.h"

void	move_cursor(struct kernel *awix, u8 x, u8 y)
{
	awix->cursor.x = x;
	awix->cursor.y = y;
}

void	print_char(u8 x, u8 y, char const c, u8 color)
{
	char	*vidmem = (char *)0xb8000;

	if (x > 79 || y > 24)
		return ;
	vidmem[((y * 80) + x) * 2] = c;
	vidmem[((y * 80) + x) * 2 + 1] = color;
}

void print_string(u8 x, u8 y, char const *s, u8 color) {
	char	*vidmem = (char *)0xb8000;

	for (; *s; s++) {
		print_char(x, y, *s, color);
		x++;
	}
}

void	line_clear(u8 y)
{
	if (y > 24)
		return ;

	for (u8 x = 0; x < 80; x++)
		print_char(x, y, ' ', 0x0F);
}

void	scroll_console()
{
	char	*vidmem = (char *)0xb8000;

	kmemmove(vidmem, vidmem + 160, 24 * 80 * 2);
	line_clear(24);
}

void	screen_clear() {
	char	*vidmem = (char *)0xb8000;

	for (int i = 0; i < 80 * 25; i++) {
		vidmem[i * 2] = ' ';
		vidmem[i * 2 + 1] = 0x0F;
	}
}
