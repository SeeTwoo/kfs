Refactor:
- ternaire or bit shifting for caps_lock ??
- assess global variable usage

Feat:
- cursor (handle arrows)
- enhance the shell with more "commands" like clear, echo, free write...
- make the console color_handling a little more complete, let's say the 16 background colors and the 16 foreground colors
- implement the clear escape code (H and J, one does cursor postion, the other clears)

Fix / Check:
- fix the print_stack command
- investigate lookup table bounds
- check safety of print string (oob writes)
