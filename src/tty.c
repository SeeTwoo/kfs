#include "events.h"
#include "ring_buffer.h"
#include "task.h"
#include "tty.h"

void	schedule();

extern struct task	tty_task;
extern struct ring	ft_stdin;
extern struct ring	events;

const char	shift_tty_chars[];
const char	tty_chars[];

void	send_character(const char *set, enum events event)
{
	while (tty_task.state == BLOCKED)
		schedule();
	char ch = set[event];
	if (ch)
		ring_push(&ft_stdin, set[event]);
}

void	consume_event(struct tty *tty, enum events event)
{
	if (!(event & 128) && event == L_SHIFT)
		tty->l_shift = 1;
	else if (event & 128 && (event & 127) == L_SHIFT)
		tty->l_shift = 0;
	else if (!(event & 128) && event == R_SHIFT)
		tty->r_shift = 1;
	else if (event & 128 && (event & 127) == R_SHIFT)
		tty->r_shift = 0;
	else if (!(event & 128) && event == CAPS_LOCK)
		tty->caps_lock = tty->caps_lock == 1 ? 0 : 1;
	else if (event & 128)
		return ;
	else if (tty->l_shift || tty->r_shift || tty->caps_lock)
		send_character(shift_tty_chars, event);
	else
		send_character(tty_chars, event);
}

void	ft_tty()
{
	struct tty	tty;

	while (1) {
		while (tty_task.state == WAITING)
			schedule();

		enum events	event;
		i32			err = ring_pop(&events, (u8 *)&event);

		consume_event(&tty, event);
	}
}

const char	tty_chars[] = {
	'\0',
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7', //yup
	'8',
	'9',
	'0',
	'-',
	'=',
	'\b',
	'\t',
	'q',
	'w',
	'e',
	'r',
	't',
	'y',
	'u',
	'i',
	'o',
	'p',
	'[',
	']',
	'\n',
	'\0',
	'a',
	's',
	'd',
	'f',
	'g',
	'h',
	'j',
	'k',
	'l',
	';',
	'\'',
	'\'',
	'\0',
	'\\',
	'z',
	'x',
	'c',
	'v',
	'b',
	'n',
	'm',
	',',
	'.',
	'/',
	'\0',
	'*',
	'\0',
	' ',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'7',
	'8',
	'9',
	'-',
	'4',
	'5',
	'6',
	'+',
	'1',
	'2',
	'3',
	'0',
	'.',
	'\0',
	'\0',

	'\n',
	'\0',
	'\'',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	127,

	'\0',
	'\0',
};

const char	shift_tty_chars[] = {
	'\0',
	'!',
	'!',
	'@',
	'#',
	'$',
	'%',
	'^',
	'&', //yup
	'*',
	'(',
	')',
	'_',
	'+',
	'\b',
	'\t',
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'{',
	'}',
	'\n',
	'\0',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	':',
	'\"',
	'~',
	'\0',
	'|',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',
	'<',
	'>',
	'?',
	'\0',
	'*',
	'\0',
	' ',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'7',
	'8',
	'9',
	'-',
	'4',
	'5',
	'6',
	'+',
	'1',
	'2',
	'3',
	'0',
	'.',
	'\0',
	'\0',

	'\n',
	'\0',
	'\'',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	'\0',
	127,

	'\0',
	'\0',
};
