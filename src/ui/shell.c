#include "console.h"
#include "inline_asm.h"
#include "io.h"
#include "kstdlib.h"
#include "panic.h"
#include "ring_buffer.h"
#include "shell.h"

void	init_shell(struct shell *sh)
{
	kmemset(sh->line, '0', LINE_SIZE);
	sh->current = 0;
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
	kputs(ft_stdout, "     -print stack\n");
/*
*/
	kprintf(ft_stdout, "%u, %x\n", 123456789, 20);
	kprintf(ft_stdout, "%u, %x\n", 0, 0);
	kprintf(ft_stdout, "%u, %x\n", 10, 16);
	kprintf(ft_stdout, "%s, %c\n", "hello", 'a');
}

extern uint32_t stack_bottom;
extern uint32_t stack_top;

/*
static void print_stack(struct ring *ft_stdout)
{
		uint32_t esp;
		__asm__ volatile("mov %%esp, %0" : "=r"(esp));

		kprintf(ft_stdout, "Stack bottom : 0x%x\n", (uint32_t)&stack_bottom);
    	kprintf(ft_stdout, "Stack top    : 0x%x\n", (uint32_t)&stack_top);
    	kprintf(ft_stdout, "Current ESP  : 0x%x\n", esp);
}
*/

void print_stack(struct ring *ft_stdout, struct console *csl) {
    uint32_t esp_val;
    uint32_t ebp_val;

    // 1. On capture l'état des registres immédiatement à l'entrée
    __asm__ volatile("mov %%esp, %0" : "=r"(esp_val));
    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp_val));

    uint32_t *current = (uint32_t *)esp_val;
    uint32_t *top     = &stack_top;
	u32			count = 0;

    kprintf(ft_stdout, "=== KERNEL STACK DUMP ===\n");
    kprintf(ft_stdout, "ESP: 0x%x | EBP: 0x%x | TOP: 0x%x\n", esp_val, ebp_val, (uint32_t)top);
    kprintf(ft_stdout, "----------------------------------------\n");

    // 2. On itère mot par mot (4 octets) d'ESP jusqu'à stack_top
    while (current < top) {
        if (current == (uint32_t *)esp_val) {
            kprintf(ft_stdout, "0x%x [ESP] : 0x%x\n", (uint32_t)current, *current);
        } else if (current == (uint32_t *)ebp_val) {
            kprintf(ft_stdout, "0x%x [EBP] : 0x%x\n", (uint32_t)current, *current);
        } else {
            kprintf(ft_stdout, "0x%x       : 0x%x\n", (uint32_t)current, *current);
        }
		ft_console(csl, ft_stdout);
        current++; // Avance de 4 octets
		count++;
    }

    kprintf(ft_stdout, "stack was %u\n", count);
    kprintf(ft_stdout, "=== END OF STACK ===\n");
}

static void	execute_line(struct shell *sh, struct ring *ft_stdout, struct console *csl)
{
	sh->line[sh->current] = '\0';
	sh->current = 0;

	kputchar(ft_stdout, '\n');
	if (kstrcmp(sh->line, "shutdown") == 0)
		shutdown();
	else if (kstrcmp(sh->line, "panic") == 0)
		panic("you like that huh ?");
	else if (kstrcmp(sh->line, "help") == 0)
		help(ft_stdout);
	else if (kstrcmp(sh->line, "print stack") == 0)
		print_stack(ft_stdout, csl);
	else if (sh->line[0])
		kputs(ft_stdout, "command not found\n");
	kputs(ft_stdout, "\x1b[32m prompt> \x1b[0m");
}

static void	backspace(struct shell *sh, struct ring *ft_stdout)
{
	if (sh->current == 0)
		return ;
	sh->current--;
	kputchar(ft_stdout, '\b');
}

static void	fill_line(struct shell *sh, char c, struct ring *ft_stdout)
{
	sh->line[sh->current] = c;
	sh->current++;
	kputchar(ft_stdout, c);
}


void	shell(struct shell *sh, struct ring *ft_stdin, struct ring *ft_stdout, struct console *csl)
{
	while (ft_stdin->count > 0) {
		char	c = ring_pop(ft_stdin);

		if (!c)
			return ;
		else if (c == '\n')
			execute_line(sh, ft_stdout, csl);
		else if (c == '\b')
			backspace(sh, ft_stdout);
		else if (sh->current < (LINE_SIZE - 2))
			fill_line(sh, c, ft_stdout);
	}
}
