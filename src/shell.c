#include "inline_asm.h"
#include "io.h"
#include "kstdlib.h"
#include "panic.h"
#include "ring_buffer.h"
#include "shell.h"

void	init_shell(struct shell *sh)
{
	kmemset(sh->line, '0', LINE_SIZE);
	sh->current = sh->line;
}

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

static void	execute_line(struct shell *sh, struct ring *ft_stdout)
{
	*(sh->current) = '\0';
	sh->current = sh->line;

	kputchar(ft_stdout, '\n');
	if (kstrcmp(sh->line, "shutdown") == 0)
		shutdown();
	else if (kstrcmp(sh->line, "panic") == 0)
		panic("you like that huh ?");
	else if (kstrcmp(sh->line, "help") == 0)
		help(ft_stdout);
	kputs(ft_stdout, "\x1b[32m prompt> \x1b[0m");
}

static void	fill_line(struct shell *sh, char c, struct ring *ft_stdout)
{
	if (c == '\b') {
		if (sh->current == sh->line)
			return ;
		sh->current--;
	} else {
		*(sh->current) = c;
		sh->current++;
	}
	kputchar(ft_stdout, c);
}


void	shell(struct shell *sh, struct ring *ft_stdin, struct ring *ft_stdout)
{
	while (ft_stdin->count > 0) {
		char	c = ring_pop(ft_stdin);

		if (!c)
			return ;
		else if (c == '\n')
			execute_line(sh, ft_stdout);
		else
			fill_line(sh, c, ft_stdout);
	}
}
