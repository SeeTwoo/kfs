#ifndef CONSOLE_HANDLING_H
#define CONSOLE_HANDLING_H

#include "ktypes.h"
#include "kernel.h"

void	move_cursor(struct kernel *, u8, u8);
void	print_string(u8 , u8 , char const *, u8);
void	print_char(u8 , u8 , char const , u8);
void	screen_clear();
void	scroll_console(struct kernel *);

#endif
