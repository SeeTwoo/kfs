#include "kstdlib.h"
#include "ktypes.h"
#include "ring_buffer.h"
///
u8 kisalpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

u8 kisdigit(char c)
{
	return c >= '0' && c <= '9';
}

i32	kstrcmp(char const *s1, char const *s2)
{
	u32	i = 0;

	for (; s1[i] && s2[i]; i++)
		if (s1[i] != s2[i])
			return s1[i] - s2[i];
	return s1[i] - s2[i];
}

void	*kmemmove(void *dest, void *src, u32 size)
{
	u8	*d = (u8 *)dest;
	u8	*s = (u8 *)src;

	if (dest > src) {
		while (size--)
			d[size] = s[size];
	} else {
		for (u32 i = 0; i < size; i++)
			d[i] = s[i];
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

	while (*s && ret <= n && out->count < RING_BUFFER_SIZE) {
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
