Build:
- Makefile must re compile when .h changed
- create libraries like the ring buffer one or the kstdlib
- split sources
- fix the rwx issue for the text section in the linker script
(/usr/bin/ld.bfd: warning: awix.elf has a LOAD segment with RWX permissions)

Refactor:
- u8 -> use char when relevant
- ternaire or bit shifting for caps_lock ??
- switch the screen to u16

Feat:
- cursor (handle arrows)
- add a message argument to the panic
- enhance the shell with "commands" like shutdown, panic, help, free write...
- implement read from a ring buffer

Fix / Check:
- investigate lookup table bounds
- check safety of print string (oob writes)
- Take a look at how to handle ring buffers being full and empty
