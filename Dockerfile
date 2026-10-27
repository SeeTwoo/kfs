FROM archlinux:latest

RUN pacman -Sy --noconfirm \
    base-devel \
    grub \
    xorriso \
    nasm \
	mtools
RUN echo 'alias ls="ls --color=auto"' >> /root/.bashrc
RUN echo 'PS1="\[\e[32m\]\W > \[\e[0m\]"' >> /root/.bashrc
RUN echo 'set number relativenumber' >> /root/.vimrc

WORKDIR /app
COPY . .
CMD ["bash"]
