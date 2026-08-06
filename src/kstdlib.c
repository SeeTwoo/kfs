#include "kstdlib.h"
#include "ktypes.h"
#include "ring_buffer.h"

//TODO : look into handling counters better
void	*kmemmove(void *dest, void *src, u32 size)
{
	u8	*dest_cast = (u8 *)dest;
	u8	*src_cast = (u8 *)src;

	if (dest > src) {
		for (i32 i = (i32)size; i >= 0; i--)
			dest_cast[size] = src_cast[size];
	} else {
		for (u32 i = 0; i < size; i++)
			dest_cast[i] = src_cast[i];
	}
	return dest;
}

void	*kmemset(void *ptr, u8 c, u32 size)
{
	u8	*ptr_cast = (u8 *)ptr;

	for (u32 i = 0; i < size; i++)
		ptr_cast[i] = c;
	return ptr;
}

u32	kstrlen(char const *s)
{
	u32	size = 0;

	while (s[size])
		size++;
	return size;
}

u32	kwrite(struct ring *out, char const *s, u32 n)
{
	u32	ret = 0;

	while (*s && out->count < RING_BUFFER_SIZE) {
		ring_push(out, *s);
		s++;
		ret++;
	}
	return ret;
}

u32	kputchar(struct ring *out, char const c)
{
	return kwrite(out, &c, 1);
}

u32	kputs(struct ring *out, char const *s)
{
	return kwrite(out, s, kstrlen(s));
}
