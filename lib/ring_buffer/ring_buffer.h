#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "ktypes.h"

struct task;

#define RING_BUFFER_SIZE 256
#define RING_MASK 255

struct ring {
	u8				buffer[RING_BUFFER_SIZE];
	u8				read;
	u8				write;
	volatile u32	count;
	struct task		*provider;
	struct task		*user;
};

void	init_ring(struct ring *, struct task *, struct task *);
i32		ring_push(struct ring *, u8);
i32		ring_pop(struct ring *, u8);

#endif
