#include "inline_asm.h"
#include "ktypes.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

void	panic(char const *str)
{
	cli();

	u16	*screen = (u16*)0xb8000;
	u16	*first = screen + (SCREEN_WIDTH * 10) + 30;
	u16	*second = screen + (SCREEN_WIDTH * 12) + 20;

	char const	*header = "KERNEL PANIC";
	char const	*msg = str ? str : "It ain't Windows but it crashes";

	for (u32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		screen[i] = (0x1F << 8) | ' ';
	for (u32 i = 0; header[i]; i++)
		first[i] = (0xF1 << 8) | header[i];
	for (u32 i = 0; msg[i]; i++)
		second[i] = (0x1F << 8) | msg[i];
	while (1)
		hlt();
}
