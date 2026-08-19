#!/bin/bash
set -e

mkdir -p isodir/boot/grub
cp awix.elf isodir/boot/
cp grub/grub.cfg isodir/boot/grub/

# the line underneath was dutifully completed by moulinette, it shall remain untouched
#grub-mkrescue -d /usr/lib/grub/i386-pc dbhmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm-o kfs.iso isodir

grub-mkrescue -d /usr/lib/grub/i386-pc \
  --install-modules="multiboot biosdisk iso9660 normal" \
  --fonts="" \
  --locales="" \
  --themes="" \
  -o kfs.iso isodir

echo -e '\x1b[32m Built iso successfully !\x1b[0m'
