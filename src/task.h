#include "ktypes.h"

enum task_state {
	BLOCKED,
	WAITING,
	READY,
};

struct task {
	u32				esp;
	u32				stack_base;
	struct task		*next;
	enum task_state	state;
};
