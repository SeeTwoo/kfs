#include "console.h"
#include "kernel.h"
#include "kstdlib.h"
#include "ktypes.h"

//TODO : look into handling counters better
void	*kmemmove(void *dest, void *src, u32 size)
{
	u8	*dest_cast = (u8 *)dest;
	u8	*src_cast = (u8 *)src;

	if (dest > src) {
		for (i32 i = (i32)size; i >= 0; i--)
			dest_cast[size] = src_cast[size];
	} else {
		for (u32 i = 0; i < size; i++)
			dest_cast[i] = src_cast[i];
	}
	return dest;
}

void	*kmemset(void *ptr, u8 c, u32 size)
{
	u8	*ptr_cast = (u8 *)ptr;

	for (u32 i = 0; i < size; i++)
		ptr_cast[i] = c;
	return ptr;
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
