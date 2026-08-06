#include "inline_asm.h"
#include "kernel.h"
#include "kstdlib.h"
#include "ring_buffer.h"
#include "tty.h"

extern struct ring kbd_ring;

void	ft_atkbd(struct ring *, u8 *);
void	ft_tty(struct tty *, struct ring *, struct ring *);
void	shell(struct kernel *, struct ring *);

void	init_awix(struct kernel *awix)
{
	awix->cursor.x = 0;
	awix->cursor.y = 0;
	awix->color = 0x0F;
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
	kmemset(&tty, '\0', sizeof(struct tty));
	while (1) {
		wait_for_interrupt();
		ft_atkbd(&events, &multibyte);
		ft_tty(&tty, &events, &ft_stdin);
		shell(&awix, &ft_stdin);
	};
}
