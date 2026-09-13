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

static void	welcome_screen(struct ring *ft_stdout)
{
	kputs(ft_stdout, "\x1b[36m");
	kputs(ft_stdout, "       :::       ::::::::\n");
	kputs(ft_stdout, "     :+:       :+:    :+:\n");
	kputs(ft_stdout, "   +#+  +:+         +#+  \n");
	kputs(ft_stdout, " +#+#+#+#+#+    +#+      \n");
	kputs(ft_stdout, "     ###     ########.fr \n");
	kputs(ft_stdout, "\x1b[0m");
	kputs(ft_stdout, "\n type \"help<Enter>\"\n\n");
	kputs(ft_stdout, "\x1b[32m prompt> \x1b[0m");
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
	init_console(&console);
	kmemset(&tty, '\0', sizeof(struct tty));
	welcome_screen(&ft_stdout);
	sti();
	while (1) {
		ft_atkbd(&events, &multibyte);
		ft_tty(&tty, &events, &ft_stdin);
		shell(&ft_stdin, &ft_stdout);
		ft_console(&console, &ft_stdout);

		cli();
		if (kbd_ring.count == 0 && events.count == 0
			&& ft_stdin.count == 0 && ft_stdout.count == 0) {
			sti();
			hlt();
		}
		sti();
	};
}
