task states
	-runnable
	-blocked
	-waiting



interrupt handler fills kbd ring with scancodes

ft_atkbd turns scancodes into events

tty turs events into stdin

shell turns stdin into stdout

console draws stdout

atkbd:

void	atkbd() {
	while (1) {
		if (ring_empty(scancodes)) {
			kbd_drv_task->state = WAITING;
			schedule();
		} else if (ring_full(events)) {
			kbd_drv_task->state = BLOCKED;
			schedule();
		} else {
			consume_scancode();
		}
	}
	panic();
}

void	tty() {
	while (1) {
		if (ring_empty(events)) {
			schedule();
		} else if (ring_full(stdin)) {
			schedule();
		} else {
			consume_events();
		}
	}
	panic();
}

void	shell() {
	while (1) {
		if (ring_empty(stdin)) {
			schedule();
		} else {
			consume_stdin();
		}
	}
	panic();
}

void	console {
	while (1) {
		if (ring_empty(stdout)) {
			schedule();
		} else {
			draw_stuff();
		}
	}
}




if we are in the scheduler and we are about
