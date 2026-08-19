# tools
CC	:= gcc
AS	:= nasm
LD	:= ld

# project structure
SRC_DIR		:= src
LIB_KSTDLIB_DIR	:= lib/kstdlib
LIB_RING_BUFFER_DIR	:= lib/ring_buffer
ASM_DIR		:= asm
LINKER_DIR	:= scripts
OBJ_DIR		:= obj

INCLUDES := -I$(SRC_DIR) -I$(LIB_KSTDLIB_DIR) -I$(LIB_RING_BUFFER_DIR)


# names
KERNEL_BIN	:= awix.elf
LINKER_SCR	:= $(LINKER_DIR)/linker.ld
ISO			:= kfs.iso


ifeq ($(find /usr/bin -type f -name grub-mkrescue),)
	BUILD_ENV := docker
else
	BUILD_ENV := native
endif


# docker constants
IMAGE_NAME		:= kfs_iso_builder
DOCKER_STAMP	:= $(OBJ_DIR)/.docker_built
DOCKER_RUN		:= docker run --rm -v $(PWD):/app  $(IMAGE_NAME)

# architecture and flags
CFLAGS := -m32 -g3 -O0 -ffreestanding -fno-stack-protector -nostdlib -MMD -MP $(INCLUDES)
CRELEASEFLAGS := -m32 -O2 -ffreestanding -fno-stack-protector -nostdlib -MMD -MP $(INCLUDES)
ASFLAGS	:= -f elf32 -g -F dwarf
ASMRELEASEFLAGS := -f elf32
LDFLAGS	:= -m elf_i386 -T $(LINKER_SCR)

# source and obj files
C_SOURCES := \
    $(SRC_DIR)/c_handler.c \
    $(SRC_DIR)/console.c \
    $(SRC_DIR)/ft_atkbd.c \
    $(SRC_DIR)/idt.c \
    $(SRC_DIR)/kloop.c \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/panic.c \
    $(SRC_DIR)/ps2_sets.c \
    $(SRC_DIR)/shell.c \
    $(SRC_DIR)/tty.c \
    $(LIB_KSTDLIB_DIR)/kstdlib.c \
    $(LIB_RING_BUFFER_DIR)/ring_buffer.c

ASM_SOURCES := \
    $(ASM_DIR)/boot.asm \
    $(ASM_DIR)/halt.asm \
    $(ASM_DIR)/io.asm \
    $(ASM_DIR)/isr.asm

# convert source paths to object paths
OBJS := \
	$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter $(SRC_DIR)/%,$(C_SOURCES))) \
	$(patsubst $(LIB_KSTDLIB_DIR)/%.c,$(OBJ_DIR)/kstdlib/%.o,$(filter $(LIB_KSTDLIB_DIR)/%,$(C_SOURCES))) \
	$(patsubst $(LIB_RING_BUFFER_DIR)/%.c,$(OBJ_DIR)/ring_buffer/%.o,$(filter $(LIB_RING_BUFFER_DIR)/%,$(C_SOURCES))) \
	$(patsubst $(ASM_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_SOURCES))



DEPS := $(OBJS:.o=.d)


# ================ RULES ==================

all: $(KERNEL_BIN)

$(KERNEL_BIN): $(OBJ_DIR) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo -e "\e[32mLinking complete: $(KERNEL_BIN)\e[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/kstdlib/%.o: $(LIB_KSTDLIB_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/ring_buffer/%.o: $(LIB_RING_BUFFER_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# iso building

iso: $(ISO)

$(ISO): $(KERNEL_BIN)
	@echo "Detected build environment: $(BUILD_ENV)"
ifeq ($(BUILD_ENV), native)
	./scripts/build_iso.sh
else
	$(MAKE) $(DOCKER_STAMP)
	$(DOCKER_RUN) ./scripts/build_iso.sh
endif

# docker rules

$(DOCKER_STAMP): Dockerfile
	@echo "Building Docker image..."
	docker build -t $(IMAGE_NAME) .
	touch $(DOCKER_STAMP)

it: $(DOCKER_STAMP)
	docker run --rm -it -v $(PWD):/app $(IMAGE_NAME) /bin/bash

# general rules

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

run-debug: $(ISO)
	qemu-system-i386 -s -S -cdrom $(ISO)

release: fclean
	$(MAKE) CFLAGS="$(CRELEASEFLAGS)" ASFLAGS="$(ASMRELEASEFLAGS)" $(KERNEL_BIN)


clean:
	rm -rf $(OBJ_DIR)
	rm -rf isodir

fclean: clean
	rm -f $(KERNEL_BIN) $(ISO)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean it iso run run-debug release re
