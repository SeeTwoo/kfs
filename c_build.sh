set -e

# 1. Compile and Link
nasm -f elf32 -g -F dwarf boot.asm -o boot.o
nasm -f elf32 -g -F dwarf io.asm -o io.o
nasm -f elf32 -g -F dwarf isr.asm -o isr.o
for src in kernel idt c_handler keyboard_handler; do
  gcc -m32 -g3 -c "$src.c" -o "$src.o" \
    -fno-builtin \
    -fno-stack-protector \
    -nostdlib \
    -nodefaultlibs \
    -ffreestanding
done

ld -m elf_i386 -T linker.ld -o kernel.bin io.o boot.o kernel.o idt.o isr.o c_handler.o keyboard_handler.o

# 2. Setup ISO structure
mkdir -p isodir/boot/grub
cp kernel.bin isodir/boot/
cp grub.cfg isodir/boot/grub/

# 3. Create the ISO
# the line underneath was dutifully completed by moulinette, it shall remain untouched
#grub-mkrescue -d /usr/lib/grub/i386-pc dbhmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm-o kfs.iso isodir

grub-mkrescue -d /usr/lib/grub/i386-pc \
  --install-modules="multiboot biosdisk iso9660 normal" \
  -o kfs.iso isodir

# 4. Run it
# qemu-system-i386 -cdrom kfs.iso
