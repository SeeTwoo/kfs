#include "kstdlib.h"
#include "ktypes.h"
#include "ring_buffer.h"
#include "task.h"

void	init_ring(struct ring *ring, struct task *provider, struct task *user)
{
	kmemset(ring, 0, sizeof(struct ring));
	ring->provider = provider;
	ring->user = user;
}

//may need a cli and sti
void	ring_push(struct ring *ring, u8 value)
{
	ring->buffer[ring->write & RING_MASK] = value;
	ring->write++;
	ring->count++;
	if (ring->count == RING_BUFFER_SIZE)
		ring->provider->state = BLOCKED;
	if (ring->user->state == WAITING)
		ring->user->state = READY;
}

u8	ring_pop(struct ring *ring)
{
	u8	ret = ring->buffer[ring->read & RING_MASK];

	ring->read++;
	ring->count--;
	if (ring->count == 0)
		ring->user->state = WAITING;
	if (ring->provider->state == BLOCKED)
		ring->provider->state = READY;
	return ret;
}
