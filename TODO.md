~~look into how to make memmove's counters great and not overflow~~

implement read from a ring buffer

create libraries like the ring buffer one or the kstdlib

investigate lookup table bounds

split sources

check safety of print string (oob writes)

Take a look at how to handle ring buffers being full and empty

add a message argument to the panic

enhance the shell with "commands" like shutdown, panic, help, free write...

fix the rwx issue for the text section in the linker script
(/usr/bin/ld.bfd: warning: awix.elf has a LOAD segment with RWX permissions)

~~check build system on school's machines~~

~~kloop & tty ft_atkdb -> loop to flush the ring~~

~~backspace~~

switch the screen to u16

cursor (handle arrows)

cursor (handle visua vga cursor)

~~capslock~~

u8 -> use char when relevant

ternaire or bit shifting for caps_lock ??

Makefile must re compile when .h changed
