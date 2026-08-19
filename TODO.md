Build:
- split sources (later)
- fix the rwx issue for the text section in the linker script
(/usr/bin/ld.bfd: warning: awix.elf has a LOAD segment with RWX permissions)

Refactor:
- ternaire or bit shifting for caps_lock ??

Feat:
- cursor (handle arrows)
- enhance the shell with "commands" like shutdown, panic, help, free write...
- implement read from a ring buffer
- make the console handle color changes with escape codes

Fix / Check:
- investigate lookup table bounds
- check safety of print string (oob writes)
- Take a look at how to handle ring buffers being full and empty
