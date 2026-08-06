#include "console.h"
#include "inline_asm.h"
#include "io.h"
#include "kernel.h"
#include "kstdlib.h"
#include "panic.h"
#include "ring_buffer.h"
#include "tty.h"

extern struct ring kbd_ring;

void	ft_atkbd(struct ring *, u8 *);
void	ft_tty(struct tty *, struct ring *, struct ring *);

void	init_awix(struct kernel *awix)
{
	awix->cursor.x = 0;
	awix->cursor.y = 0;
	awix->color = 0x0F;
}

void	kputchar(struct kernel *awix, char const c)
{
	char	*screen = (char *)0xb8000;

	if (awix->cursor.x == 80)
		move_cursor(awix, 0, awix->cursor.y + 1);
	if (awix->cursor.y == 25) {
		scroll_console(awix);
		awix->cursor.y = 24;
	}
	print_char(awix->cursor.x, awix->cursor.y, c, awix->color);
	awix->cursor.x++;
}

void	kputs(struct kernel *awix, char const *s)
{
	for (; *s; s++)
		kputchar(awix, *s);
}

void	new_line(struct kernel *awix)
{
	if (awix->cursor.y == 24) {
		scroll_console(awix);
		move_cursor(awix, 0, 24);
		return ;
	}
	move_cursor(awix, 0, awix->cursor.y + 1);
}

void	shutdown()
{
	cli();
	outw(0x604, 0x2000);
	hlt();
	panic();
}

void	wait_for_interrupt(void)
{
	cli();
	if (kbd_ring.count == 0) {
		sti();
		hlt();
	}
	sti();
}

void	shell(struct kernel *awix, struct ring *ft_stdin)
{
	if (ft_stdin->count == 0)
		return ;
	u8	c = ring_pop(ft_stdin);

	if (!c)
		return ;
	else if (c == '\n')
		new_line(awix);
	else if (c == 127)
		shutdown();
	else
		kputchar(awix, c);
}

void	kloop()
{
	struct kernel	awix;
	struct ring		events;
	struct ring		ft_stdin;
	struct tty		tty;
	u8				multibyte = 0;

	init_awix(&awix);
	init_ring(&events);
	init_ring(&ft_stdin);
	memset(&tty, '\0', sizeof(struct tty));
	while (1) {
		wait_for_interrupt();
		ft_atkbd(&events, &multibyte);
		ft_tty(&tty, &events, &ft_stdin);
		shell(&awix, &ft_stdin);
	};
}
