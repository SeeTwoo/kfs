#include "ktypes.h"
#include "ring_buffer.h"
#include "kstdlib.h"

void	init_ring(struct ring *ring)
{
	kmemset(ring->buffer, 0, RING_BUFFER_SIZE);
	ring->read = 0;
	ring->write = 0;
	ring->count = 0;
}

void	ring_push(struct ring *ring, u8 value)
{
	ring->buffer[ring->write & RING_MASK] = value;
	ring->write++;
	ring->count++;
}

u8	ring_pop(struct ring *ring)
{
	u8	ret = ring->buffer[ring->read & RING_MASK];
	ring->read++;
	ring->count--;
	return ret;
}
