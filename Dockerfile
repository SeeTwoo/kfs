FROM archlinux:latest

RUN pacman -Sy --noconfirm \
    base-devel \
    vim \ 
    grub \
    xorriso \
    nasm \
	mtools \
    qemu-full
RUN echo 'alias ls="ls --color=auto"' >> /root/.bashrc
RUN echo 'PS1="\[\e[32m\]\W > \[\e[0m\]"' >> /root/.bashrc
RUN echo 'set number relativenumber' >> /root/.vimrc

WORKDIR /app
COPY . .
CMD ["bash"]
