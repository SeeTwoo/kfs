void	kmain(void)
{
	char	*vidmem = (char *)0xb8000;

	vidmem[0] = 'A';
	vidmem[1] = 0x0F;
	while (1);
}
