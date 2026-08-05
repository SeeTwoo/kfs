#include "console_handling.h"

void	panic()
{
	asm volatile("cli");
	screen_clear();
	print_string(4, 12, "panic", 0x0F);
	while (1)
		asm volatile("hlt");
}
