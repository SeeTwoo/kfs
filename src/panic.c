#include "console_handling.h"
#include "inline_asm.h"

void	panic()
{
	cli();
	screen_clear();
	print_string(4, 12, "panic", 0x0F);
	while (1)
		hlt();
}
