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

u8	ring_is_full(struct ring *ring)
{
	return ring->count == RING_BUFFER_SIZE;
}

u8	ring_is_empty(struct ring *ring)
{
	return ring->count == 0;
}

u32	get_ring_count(struct ring *ring)
{
	return ring->count;
}

//may need a cli and sti
i32	ring_push(struct ring *ring, u8 value)
{
	if (ring->count == RING_BUFFER_SIZE)
		return -1;
	ring->buffer[ring->write & RING_MASK] = value;
	ring->write++;
	ring->count++;
	if (ring->count == RING_BUFFER_SIZE)
		ring->provider->state = BLOCKED;
	if (ring->user->state == WAITING)
		ring->user->state = READY;
	return 0;
}

i32	ring_pop(struct ring *ring, u8 *value)
{
	if (ring->count == 0)
		return -1;
	*value = ring->buffer[ring->read & RING_MASK];
	ring->read++;
	ring->count--;
	if (ring->count == 0)
		ring->user->state = WAITING;
	if (ring->provider->state == BLOCKED)
		ring->provider->state = READY;
	return 0;
}
