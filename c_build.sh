# 1. Compile and Link
nasm -f elf32 boot.asm -o boot.o
gcc -m32 -c kernel.c -o kernel.o \
	-fno-builtin \
	-fno-stack-protector \
	-nostdlib \
	-nodefaultlibs \
	-ffreestanding

ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o

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
