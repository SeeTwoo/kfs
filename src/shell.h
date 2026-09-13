#ifndef SHELL_H
#define SHELL_H

#ifndef LINE_SIZE
#define LINE_SIZE 64
#endif

struct shell {
	char	line[LINE_SIZE];
	char	*current;
};

#endif
