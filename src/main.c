#include "console.h"
#include "inline_asm.h"
#include "interrupts.h"
#include "io.h"
#include "kstdlib.h"
#include "ktypes.h"
#include "panic.h"
#include "ring_buffer.h"
#include "shell.h"
#include "tty.h"

extern void	halt();
extern void isr21(void);
void		ft_atkbd(struct ring *, u8 *);
void		ft_tty(struct tty *, struct ring *, struct ring *);
void		init_shell(struct shell *);
void		shell(struct shell *, struct ring *, struct ring *);
void		console(struct console *, struct ring *);

struct ring kbd_ring;

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
	struct shell	sh;
	struct console	console;
	u8				multibyte = 0;

	init_ring(&events);
	init_ring(&ft_stdin);
	init_ring(&ft_stdout);
	init_shell(&sh);
	init_console(&console);
	kmemset(&tty, '\0', sizeof(struct tty));
	welcome_screen(&ft_stdout);
	sti();
	while (1) {
		ft_atkbd(&events, &multibyte);
		ft_tty(&tty, &events, &ft_stdin);
		shell(&sh, &ft_stdin, &ft_stdout);
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


static void	pic_remap()
{
	outb(0x20, 0x11); outb(0xA0, 0x11);
	outb(0x21, 0x20); outb(0xA1, 0x28);
	outb(0x21, 0x04); outb(0xA1, 0x02);
	outb(0x21, 0x01); outb(0xA1, 0x01);
	outb(0x21, 0xFD); outb(0xA1, 0xFF);
}

void kmain(void)
{
	pic_remap();
	idt_init();
	init_ring(&kbd_ring);
	set_idt_gate(0x21, (u32)isr21);
	kloop();
	panic("Got out of kloop (play lottery) (T_T) ");
}

// SEVEN LINES TALL
//         :::     ::::::::
//       :+:     :+:    :+:
//     +:+ +:+        +:+
//   +#+  +:+       +#+
// +#+#+#+#+#+   +#+
//     #+#    #+#
//    ###   ########.fr
//
// FIVES LINES TALL
//       :::       ::::::::
//     :+:       :+:    :+:
//   +#+  +:+         +#+
// +#+#+#+#+#+    +#+
//     ###     ########.fr
