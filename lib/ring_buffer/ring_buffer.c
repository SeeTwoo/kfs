#include "kstdlib.h"
#include "ktypes.h"
#include "ring_buffer.h"
#include "task.h"

void	init_ring(struct ring *ring)
{
	kmemset(ring, 0, sizeof(struct ring));
}

//may need a cli and sti
void	ring_push(struct ring *ring, u8 value)
{
	if (ring->count == RING_BUFFER_SIZE) {
		ring->provider->state = BLOCKED;
		return ;
	}
	ring->buffer[ring->write & RING_MASK] = value;
	ring->write++;
	ring->count++;
}

u8	ring_pop(struct ring *ring)
{
	if (ring->count == 0) {
		ring->user->state = WAITING;
		return ;
	}
	u8	ret = ring->buffer[ring->read & RING_MASK];
	ring->read++;
	ring->count--;
	return ret;
}
