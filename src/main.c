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

struct task ft_atkbd_task;
struct task tty_task;
struct task shell_task
struct task console_task;
struct task	idle_task;

struct ring	events;
struct ring	ft_stdin;
struct ring	ft_stdout;

static void	pic_remap()
{
	outb(0x20, 0x11); outb(0xA0, 0x11);
	outb(0x21, 0x20); outb(0xA1, 0x28);
	outb(0x21, 0x04); outb(0xA1, 0x02);
	outb(0x21, 0x01); outb(0xA1, 0x01);
	outb(0x21, 0xFD); outb(0xA1, 0xFF);
}

void	idle(void)
{
	hlt();
	schedule();
}

void	init_tasks(void)
{
	ft_atkbd_task->next = tty;
	tty_task->next = shell;
	shell_task->next = console;
	console_task->next = idle_task;
	idle_task->next = ft_atkbd_task

	idle_task->stack_base = 20480;
	ft_atkbd_task->stack_base = 16384;
	tty_task->stack_base = 12288;
	shell_task->stack_base = 8192;
	console_task->next = 4096;

	idle_task->esp = 20480;
	ft_atkbd_task->esp = 16384;
	tty_task->esp = 12288;
	shell_task->esp = 8192;
	console_task->esp = 4096;
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
	u32			previous_esp = current_task->esp;
	struct task	*temp = current_task->next;
	u8			waiting = 1;

	while (temp->state != READY) {
		if (temp->state != WAITING)
			waiting = 0;
		if (temp = current_task)
			break ;
		current_task = current_task->next;
	}
	if (waiting)
		switch_to(idle)
	switch_to(previous_esp, current_task->esp);
}

void kmain(void)
{
	pic_remap();
	idt_init();
	init_ring(&kbd_ring);
	set_idt_gate(0x21, (u32)isr21);

	init_tasks();
	init_ring(&events, &ft_atkbd_task, &tty_task);
	init_ring(&ft_stdin, &tty_task, &shell_task);
	init_ring(&ft_stdout, &shell_task, &console_task);
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
