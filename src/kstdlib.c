#include "kstdlib.h"
#include "ktypes.h"

void	*memset(u8 *ptr, u8 c, u32 size)
{
	for (u32 i = 0; i < size; i++)
		ptr[i] = c;
	return ptr;
}

i32 is_print(u8 c)
{
	return c >= 'a' && c <= 'z';
}
