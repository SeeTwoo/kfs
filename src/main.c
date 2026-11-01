#include "console.h"
#include "interrupts.h"
#include "io.h"
#include "kstdlib.h"
#include "ktypes.h"
#include "panic.h"
#include "ring_buffer.h"
#include "shell.h"
#include "task.h"
#include "tty.h"

extern void isr21(void);
void		ft_atkbd(struct ring *, u8 *);
void		ft_tty(struct tty *, struct ring *, struct ring *);
void		shell(struct shell *, struct ring *, struct ring *);
void		console(struct console *, struct ring *);

struct ring kbd_ring;
struct task	*current_task;

static void	pic_remap()
{
	outb(0x20, 0x11); outb(0xA0, 0x11);
	outb(0x21, 0x20); outb(0xA1, 0x28);
	outb(0x21, 0x04); outb(0xA1, 0x02);
	outb(0x21, 0x01); outb(0xA1, 0x01);
	outb(0x21, 0xFD); outb(0xA1, 0xFF);
}

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

void	schedule()
{
	u32	previous_esp = current_task->esp;

	while (current_task->state != READY) {
		current_task = current_task->next;
	}
	switch_to(previous_esp, current_task->esp);
}

void kmain(void)
{
	struct ring		events;
	struct ring		ft_stdin;
	struct ring		ft_stdout;

	pic_remap();
	idt_init();
	init_ring(&kbd_ring);
	set_idt_gate(0x21, (u32)isr21);

	init_ring(&events);
	init_ring(&ft_stdin);
	init_ring(&ft_stdout);
	welcome_screen(&ft_stdout);
	schedule();
	panic("the cpu escaped the scheduler (play the lottery) (T_T) ");
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
