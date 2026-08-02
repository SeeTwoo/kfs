void	screen_clear(char *vidmem)
{
	for (int i = 0; i < 80 * 25; i++) {
		vidmem[i * 2] = ' ';
		vidmem[i * 2 + 1] = 0x0F;
	}
}

void	print_string(int x, int y, char const *s, char *vidmem)
{
	for (; *s; s++) {
		vidmem[((y * 80) + x) * 2] = *s;
		vidmem[((y * 80) + x) * 2  + 1] = 0x01;
		x++;
	}
}

void	kmain(void)
{
	char	*vidmem = (char *)0xb8000;

	screen_clear(vidmem);
	print_string(20, 12, "42", vidmem);
	while (1);
}
