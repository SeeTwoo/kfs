#ifndef INLINE_ASM_H
#define INLINE_ASM_H

static inline void	hlt()
{
	__asm__ __volatile__("hlt");
}

static inline void	cli()
{
	__asm__ __volatile__("cli");
}

static inline void	sti()
{
	__asm__ __volatile__("sti");
}

#endif
