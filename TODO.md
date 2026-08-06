~~isr.asm -> stubs + load_idt()~~

pic.c -> remap du 8259 (IRQ1 doit etre a 32 ou 33 jspu et plus a 2 ), sinon ca fait doublon els interrupt
type division par zero

keyboard.c -> handler IRQ1 -> concretement on lit le charactere on l'ecrit ??

kernel.c -> appeller les fonctions(fait normalement)
build.sh-> ajouter les fihciers




look into how to make memmove's counters great and not overflow

keep breaking down the system by separating the shell and the console

we will need to create a stdout in the middle

this would be the occasion to implement read and write which would be wrappers to pop or push several values
at a time

create libraries like the ring buffer one or the kstdlib

investigate lookup table bounds

split sources

doing a round of static and const check

remove parameter names in the prototypes

check safety of print string (oob writes)

Take a look at how to handle ring buffers being full and empty

Make sure panic works properly

enhance the shell with "commands" like shutdown, panic, help, free write...
