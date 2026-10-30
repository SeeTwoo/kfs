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

extern u32 stack_top;

void	backtrace(struct ring *ft_stdout, struct console *csl)
{
    u32 *ebp;
    u8	frame = 0;

    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp));

    kprintf(ft_stdout, "=== CALL STACK BACKTRACE ===\n");

    while (ebp != 0 && (u32)ebp < (u32)&stack_top) {
        u32 saved_ebp = ebp[0];
        u32 ret_addr  = ebp[1];

        kprintf(ft_stdout, "#%u [EBP: 0x%x] Return Addr: 0x%x\n", frame++, (u32)ebp, ret_addr);
        ft_console(csl, ft_stdout);

        ebp = (u32*)saved_ebp;
    }
    kprintf(ft_stdout, "============================\n");
}

/*
void	print_stack(struct ring *ft_stdout, struct console *csl)
{
	u32 esp_val, ebp_val;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp_val));
    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp_val));

    u32 *current = (u32*)esp_val;
    u32 *top     = &stack_top;

    kprintf(ft_stdout, "=== STACK DUMP (0x%x -> 0x%x) ===\n", esp_val, (u32)top);

    while (current < top) {
        const char *tag = "          ";

        u32 *frame = (u32*)ebp_val;
        while (frame != 0x0 && (u32)frame < (u32)top) {
            if (current == frame) {
                tag = "<- [SAVED EBP]";
                break;
            } else if (current == frame + 1) {
                tag = "<- [RET ADDR] ";
                break;
            }
            frame = (u32*)(*frame);
        }

        if (current == (u32*)esp_val) {
            tag = "<- [ESP]      ";
        }

        kprintf(ft_stdout, "0x%x : 0x%x %s\n", (u32)current, *current, tag);
        ft_console(csl, ft_stdout);
        current++;
    }
}
*/

void	print_stack(struct ring *ft_stdout, struct console *csl)
{
	u32 esp_val;
	__asm__ volatile("mov %%esp, %0" : "=r"(esp_val));

	u32	*current = (u32*)esp_val;
	u32	*top = &stack_top;

	while (current < top) {
		kprintf(ft_stdout, "0x%x : 0x%x\n", (u32)current, *current);
		ft_console(csl, ft_stdout);
		current++;
	}
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
	else if (kstrcmp(sh->line, "backtrace") == 0)
		backtrace(ft_stdout, csl);
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
