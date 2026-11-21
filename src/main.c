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
void		init_task(struct task *, struct task *, u32, void (*)(void));

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

void	init_task(struct task *t, struct task *next, u32 stack_base, void (*entry_function)(void))
{
	t->stack_base = stack_base;
	u32	*stack = (u32*)(t->stack_base + 4096);

	*(--stack) = (u32)entry_function;
	*(--stack) = 0;
	*(--stack) = 0;
	*(--stack) = 0;
	*(--stack) = 0;
	t->esp = (u32)stack;
	t->state = WAITING;
	t->next = next;
}

void	init_tasks(void)
{
	init_task(&ft_atkbd_task, &tty_task, (u32)ft_atkbd_stack, ft_atkbd);
	init_task(&tty_task, &shell_task, (u32)tty_stack, ft_tty);
	init_task(&shell_task, &console_task, (u32)shell_stack, shell);
	init_task(&console_task, &ft_atkbd_task, (u32)console_stack, ft_console);
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
	struct task	*temp = &ft_atkbd_task;
	struct task	*start = &ft_atkbd_task;

	do {
		if (temp->state == READY) {
			current_task = temp;
			switch_to((u32 *)previous_esp, (u32 *)temp->esp);
			return ;
		}
		temp = temp->next;
	} while (temp != start);
	current_task = &boot_idle_task;
	switch_to((u32 *)previous_esp, (u32 *)boot_idle_task.esp);
}

void kmain(void)
{
	pic_remap();
	idt_init();
	set_idt_gate(0x21, (u32)isr21);

	current_task = &boot_idle_task;
	init_tasks();
	init_ring(&kbd_ring, &dummy_type_task, &ft_atkbd_task);
	init_ring(&events, &ft_atkbd_task, &tty_task);
	init_ring(&ft_stdin, &tty_task, &shell_task);
	init_ring(&ft_stdout, &shell_task, &console_task);
	welcome_screen(&ft_stdout);
	sti();
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
