Build:
- thin the dockerfile out for faster builds (qemu is not necessary for example I think)
- split sources (later)
- makefile eviter SRRC_DIR a chaque fois
- fix the rwx issue for the text section in the linker script
(/usr/bin/ld.bfd: warning: awix.elf has a LOAD segment with RWX permissions)

Refactor:
- ternaire or bit shifting for caps_lock ??
- make the ring buffer fully encapsulated, for example with "ring_is_full" and "ring_is_empty" methods and using opaque pointers

Feat:
- cursor (handle arrows)
- enhance the shell with more "commands" like clear, echo, free write...
- enhance the shell so it tolerates backspace gracefully (not write garbage in the command...)
- enhance the shell with a "command not found" message
- implement read from a ring buffer
- make the console color_handling a little more complete, let's say the 16 background colors and the 16 foreground colors
- implement the clear escape code (H and J, one does cursor postion, the other clears)

Fix / Check:
- investigate lookup table bounds
- check safety of print string (oob writes)
- Take a look at how to handle ring buffers being full and empty
- check what happens if the line of the shell reaches the limit of the buffer, do safety stuff
