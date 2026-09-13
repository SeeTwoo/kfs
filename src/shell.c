#include "inline_asm.h"
#include "io.h"
#include "kstdlib.h"
#include "panic.h"
#include "ring_buffer.h"

static void	shutdown()
{
	cli();
	outw(0x604, 0x2000);
	hlt();
	panic("Shutdown failed ???????????");
}

static void	help(struct ring *ft_stdout)
{
	kputs(ft_stdout, " hello !\n");
	kputs(ft_stdout, " this kernel is VERY limited but here is stuff you can try :\n");
	kputs(ft_stdout, "     -help\n");
	kputs(ft_stdout, "     -shutdown\n");
	kputs(ft_stdout, "     -panic\n");
	kputs(ft_stdout, "\n");
}

static char	line[64];
static char	*current = line;
static u8	need_prompt = 1;

i32	kstrcmp(char const *s1, char const *s2)
{
	u32	i = 0;

	for (; s1[i] && s2[i]; i++)
		if (s1[i] != s2[i])
			return s1[i] - s2[i];
	return s1[i] - s2[i];
}

static void	execute_line(struct ring *ft_stdout)
{
	*current = '\0';
	current = line;
	need_prompt = 1;

	kputchar(ft_stdout, '\n');
	if (kstrcmp(line, "shutdown") == 0)
		shutdown();
	else if (kstrcmp(line, "panic") == 0)
		panic("you like that huh ?");
	else if (kstrcmp(line, "help") == 0)
		help(ft_stdout);
}

static void	fill_line(char c, struct ring *ft_stdout)
{
	*current = c;
	current++;
	kputchar(ft_stdout, c);
}


void	shell(struct ring *ft_stdin, struct ring *ft_stdout)
{
	while (ft_stdin->count > 0) {
		if (need_prompt) {
			kputs(ft_stdout, "\x1b[32m prompt> \x1b[0m");
			need_prompt = 0;
		}
		char	c = ring_pop(ft_stdin);

		if (!c)
			return ;
		else if (c == '\n')
			execute_line(ft_stdout);
		else if (c == 127)
			shutdown();
		else
			fill_line(c, ft_stdout);
	}
}
