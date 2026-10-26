#include "idt.h"
#include "io.h"
#include "ktypes.h"
#include "panic.h"
#include "ring_buffer.h"

extern void	halt();
extern void isr21(void);
void		kloop();


struct ring kbd_ring;

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
