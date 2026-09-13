Build:
- thin the dockerfile out for faster builds (qemu is not necessary for example I think)
- split sources (later)
- makefile eviter SRRC_DIR a chaque fois
- fix the rwx issue for the text section in the linker script
(/usr/bin/ld.bfd: warning: awix.elf has a LOAD segment with RWX permissions)

Refactor:
- ternaire or bit shifting for caps_lock ??
- make the ring buffer fully encapsulated, for example with "ring_is_full" and "ring_is_empty" methods and using opaque pointers
- put kstrcmp in the stdlib

Feat:
- cursor (handle arrows)
- enhance the shell with "commands" like shutdown, panic, help, clear, echo, free write...
- implement read from a ring buffer
- make the console color_handling a little more complete, let's say the 16 background colors and the 16 foreground colors
- implement the clear escape code (H and J, one does cursor postion, the other clears)

Fix / Check:
- investigate lookup table bounds
- check safety of print string (oob writes)
- Take a look at how to handle ring buffers being full and empty
