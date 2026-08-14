#include "inline_asm.h"
#include "io.h"
#include "kstdlib.h"
#include "panic.h"
#include "ring_buffer.h"

static void	shutdown()
{
	cli();
	outw(0x604, 0x2000);
	hlt();
	panic("Shutdown failed ???????????");
}

void	shell(struct ring *ft_stdin, struct ring *ft_stdout)
{
	while (ft_stdin->count > 0) {
		u8	c = ring_pop(ft_stdin);

		if (!c)
			return ;
		else if (c == 127)
			shutdown();
		else
			kputchar(ft_stdout, c);
	}
}
