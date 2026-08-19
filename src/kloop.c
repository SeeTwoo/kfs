#include "console.h"
#include "inline_asm.h"
#include "kstdlib.h"
#include "ring_buffer.h"
#include "tty.h"

extern struct ring kbd_ring;

void	ft_atkbd(struct ring *, u8 *);
void	ft_tty(struct tty *, struct ring *, struct ring *);
void	shell(struct ring *, struct ring *);
void	console(struct console *, struct ring *);

static void	wait_for_interrupt(void)
{
	cli();
	if (kbd_ring.count == 0) {
		sti();
		hlt();
	}
	sti();
}

static void	screen_start(struct console *csl, struct ring *ft_stdout)
{
	init_console(csl);
	screen_clear(csl);
	csl->color = 0x03;
	kputs(ft_stdout, "       :::       ::::::::\n");
	kputs(ft_stdout, "     :+:       :+:    :+:\n");
	kputs(ft_stdout, "   +#+  +:+         +#+  \n");
	kputs(ft_stdout, " +#+#+#+#+#+    +#+      \n");
	kputs(ft_stdout, "     ###     ########.fr \n");
	kputs(ft_stdout, "\n\n\n\n\n\n\n\n\n\n\n");
	ft_console(csl, ft_stdout);
	csl->color = 0x0F;
}

void	kloop()
{
	struct ring		events;
	struct ring		ft_stdin;
	struct ring		ft_stdout;
	struct tty		tty;
	struct console	console;
	u8				multibyte = 0;

	init_ring(&events);
	init_ring(&ft_stdin);
	init_ring(&ft_stdout);
	kmemset(&tty, '\0', sizeof(struct tty));
	screen_start(&console, &ft_stdout);
	sti();
	while (1) {
		wait_for_interrupt();
		ft_atkbd(&events, &multibyte);
		ft_tty(&tty, &events, &ft_stdin);
		shell(&ft_stdin, &ft_stdout);
		ft_console(&console, &ft_stdout);
	};
}
