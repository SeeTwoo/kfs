#include "inline_asm.h"
#include "interrupts.h"
#include "io.h"
#include "kstdlib.h"
#include "ktypes.h"
#include "panic.h"
#include "ring_buffer.h"
#include "task.h"

extern void isr21(void);
void		ft_atkbd();
void		ft_tty();
void		shell();
void		ft_console();

void		switch_to(u32 *, u32 *);
void		init_task(struct task *, void (*)(void));

struct task	*current_task;

struct task	boot_idle_task;

struct task ft_atkbd_task;
struct task tty_task;
struct task shell_task;
struct task console_task;

struct task	dummy_type_task;

struct ring kbd_ring;
struct ring	events;
struct ring	ft_stdin;
struct ring	ft_stdout;

u8	ft_atkbd_stack[4096] __attribute__((aligned(16)));
u8	tty_stack[4096] __attribute__((aligned(16)));
u8	shell_stack[4096] __attribute__((aligned(16)));
u8	console_stack[4096] __attribute__((aligned(16)));

static void	pic_remap()
{
	outb(0x20, 0x11); outb(0xA0, 0x11);
	outb(0x21, 0x20); outb(0xA1, 0x28);
	outb(0x21, 0x04); outb(0xA1, 0x02);
	outb(0x21, 0x01); outb(0xA1, 0x01);
	outb(0x21, 0xFD); outb(0xA1, 0xFF);
}

void	init_tasks(void)
{
	ft_atkbd_task.next = &tty_task;
	ft_atkbd_task.stack_base = (u32)ft_atkbd_stack;
	ft_atkbd_task.state = WAITING;
	init_task(&ft_atkbd_task, ft_atkbd);

	tty_task.next = &shell_task;
	tty_task.stack_base = (u32)tty_stack;
	ft_atkbd_task.state = WAITING;
	init_task(&tty_task, ft_tty);

	shell_task.next = &console_task;
	shell_task.stack_base = (u32)shell_stack;
	ft_atkbd_task.state = WAITING;
	init_task(&shell_task, shell);

	console_task.next = &ft_atkbd_task;
	console_task.stack_base = (u32)console_stack;
	ft_atkbd_task.state = WAITING;
	init_task(&console_task, ft_console);

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
	u32			*previous_esp = &(current_task->esp);
	struct task	*temp = current_task->next;
	u8			waiting = 1;

	while (temp->state != READY) {
		if (temp->state != WAITING)
			waiting = 0;
		if (temp == current_task)
			break ;
		temp = temp->next;
	}
	if (temp->state == READY) {
		switch_to((u32 *)previous_esp, (u32 *)temp->esp);
		return ;
	}
	if (waiting) {
		switch_to((u32 *)previous_esp, (u32 *)&(boot_idle_task.esp));
		return ;
	}
}

void kmain(void)
{
	pic_remap();
	idt_init();
	set_idt_gate(0x21, (u32)isr21);

	init_tasks();
	init_ring(&kbd_ring, &dummy_type_task, &ft_atkbd_task);
	init_ring(&events, &ft_atkbd_task, &tty_task);
	init_ring(&ft_stdin, &tty_task, &shell_task);
	init_ring(&ft_stdout, &shell_task, &console_task);
	welcome_screen(&ft_stdout);
	while (1) {
		schedule();
		hlt();
	}
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
