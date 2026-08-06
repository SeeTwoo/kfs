#ifndef IO_H
#define IO_H

#include "ktypes.h"

extern u8	inb(u16 port);
extern void outb(u16, u16);
extern void outw(u16, u16);

#endif
