idt.c -> implement the 2 fnctions 

isr.asm -> stubs

pic.c -> remap du 8259

keyboard.c -> handler IRQ1
EOI obligatoire : outb(0x20, 0x20) — sans ça le PIC ne délivre plus rien.

kernel.c -> appeller les fonctions 
build.sh-> ajouter les fihciers
