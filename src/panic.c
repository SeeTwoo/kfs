#include "inline_asm.h"
#include "ktypes.h"

void	panic()
{
	cli();

	u16	*screen = (u16*)0xb8000;
	u16	*first = screen + (((80 * 10) + 10) * 2);
	u16	*second = screen + (((80 * 12) + 5) * 2);

	char const	*header = "KERNEL PANIC";
	char const	*msg = "It ain't Windows but it crashes";

	for (u32 i = 0; i < 80 * 25; i++)
		screen[i] = (0x1F << 8) | ' ';
	for (u32 i = 0; header[i]; i++)
		first[i] = (0xF1 << 8) | header[i];
	for (u32 i = 0; msg[i]; i++)
		second[i] = (0x1F << 8) | msg[i];
	while (1)
		hlt();
}
