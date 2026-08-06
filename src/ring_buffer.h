#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "ktypes.h"


#define RING_BUFFER_SIZE 16
#define RING_MASK 15

struct ring {
	u8			buffer[RING_BUFFER_SIZE];
	u8			read;
	u8			write;
	volatile u8	count;
};

void	init_ring(struct ring *ring);
void	ring_push(struct ring *ring, u8 value);
u8		ring_pop(struct ring *ring);

#endif
