#include "kstdlib.h"
#include "ktypes.h"

//TODO : look into handling counters better
void	*memmove(void *dest, void *src, u32 size)
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

void	*memset(void *ptr, u8 c, u32 size)
{
	u8	*ptr_cast = (u8 *)ptr;

	for (u32 i = 0; i < size; i++)
		ptr_cast[i] = c;
	return ptr;
}

i32 is_print(u8 c)
{
	return c >= 'a' && c <= 'z';
}
