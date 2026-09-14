#ifndef STDLIB_H
#define STDLIB_H

#include "ktypes.h"
#include "ring_buffer.h"
///

u8		kisalpha(char c);
u8		kisdigit(char c);
void	*kmemmove(void *, void *, u32);
void	*kmemset(void *, u8, u32);
u32		kputchar(struct ring *, char const);
u32		kputs(struct ring *, char const *);
i32		kstrcmp(char const *s1, char const *s2);
u32		kstrlen(char const *);
u32		kwrite(struct ring *, char const *, u32);

#endif
