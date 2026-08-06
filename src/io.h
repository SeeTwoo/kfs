#ifndef IO_H
#define IO_H

#include "ktypes.h"

extern u8	inb(u16 port);
extern void outb(uint16_t port, uint16_t val);
extern void outw(u16, u16);

#endif
