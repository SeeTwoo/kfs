~~isr.asm -> stubs + load_idt()~~

pic.c -> remap du 8259 (IRQ1 doit etre a 32 ou 33 jspu et plus a 2 ), sinon ca fait doublon els interrupt
type division par zero

keyboard.c -> handler IRQ1 -> concretement on lit le charactere on l'ecrit ??

EOI obligatoire : outb(0x20, 0x20) — sans ça le PIC ne délivre plus rien.

kernel.c -> appeller les fonctions(fait normalement)
build.sh-> ajouter les fihciers



target remote :1234
symbol-file kernel.bin
set architecture i386
break kmain
c


look into how to make memmove's counters great and not overflow
