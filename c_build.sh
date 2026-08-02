# 1. Compile and Link
nasm -f elf32 boot.asm -o boot.o
gcc -m32 -c kernel.c -o kernel.o \
	-fno-builtin \
	-fno-stack-protector \
	-fno-rtti \
	-nostdlib \
	-nodefaultlibs \
	-ffreestanding

ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o

# 2. Setup ISO structure
mkdir -p isodir/boot/grub
cp kernel.bin isodir/boot/
cp grub.cfg isodir/boot/grub/

# 3. Create the ISO
grub-mkrescue -o kfs.iso isodir

# 4. Run it
# qemu-system-i386 -cdrom kfs.iso
