#include "ktypes.h"
#include "task.h"

void	init_task(struct task *t, void (*entry_function)(void))
{
	u32	*stack = (u32*)(t->stack_base + 4096);

	*(--stack) = (u32)entry_function;
	*(--stack) = 0;
	*(--stack) = 0;
	*(--stack) = 0;
	*(--stack) = 0;
	t->esp = (u32)stack;
}
