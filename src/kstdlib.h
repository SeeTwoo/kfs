#ifndef STDLIB_H
#define STDLIB_H

#include "ktypes.h"
#include "ring_buffer.h"

void	*kmemmove(void *, void *, u32);
void	*kmemset(void *, u8, u32 );
u32		kputchar(struct ring *, char const);
u32		kputs(struct ring *, char const *);
u32		kstrlen(char const *);
u32		kwrite(struct ring *, char const *, u32);

#endif
