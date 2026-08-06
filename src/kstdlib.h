#ifndef STDLIB_H
#define STDLIB_H

#include "ktypes.h"
#include "kernel.h"

void	*kmemmove(void *, void *, u32);
void	*kmemset(void *, u8, u32 );
void	kputchar(struct kernel *, char const c);
void	kputs(struct kernel *, char const *s);

#endif
