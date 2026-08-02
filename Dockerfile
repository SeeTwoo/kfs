FROM archlinux:latest


RUN pacman -Sy --noconfirm \
    base-devel \
    grub \
    xorriso \
    nasm \
	mtools \
    qemu-full

WORKDIR /app
COPY . .
CMD ["bash"]
