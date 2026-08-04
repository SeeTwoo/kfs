#ifndef KERNEL_H
#define KERNEL_H

#include "ktypes.h"

struct cursor {
	u8	x;
	u8	y;
};

struct kernel {
	struct cursor	cursor;
	u8				color;
};

#endif
