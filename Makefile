# tools
CC	:= gcc
AS	:= nasm
LD	:= ld

# project structure
SRC_DIR		:= src
ASM_DIR		:= asm
LINKER_DIR	:= scripts
OBJ_DIR		:= obj

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
DOCKER_RUN		:= docker run --rm -v $(PWD):/app --userns=keep-id $(IMAGE_NAME)

# architecture and flags
CFLAGS := -m32 -g3 -O0 -ffreestanding -fno-stack-protector -nostdlib -MMD -MP
CRELEASEFLAGS := -m32 -O2 -ffreestanding -fno-stack-protector -nostdlib -MMD -MP
ASFLAGS	:= -f elf32 -g -F dwarf
ASMRELEASEFLAGS := -f elf32
LDFLAGS	:= -m elf_i386 -T $(LINKER_SCR)

# source and obj files
C_SOURCES	:= $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES	:= $(wildcard $(ASM_DIR)/*.asm)

# convert source paths to object paths
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
OBJS += $(patsubst $(ASM_DIR)/%.asm, $(OBJ_DIR)/%.o, $(ASM_SOURCES))

DEPS := $(OBJS:.o=.d)


# ================ RULES ==================

all: $(KERNEL_BIN)

$(KERNEL_BIN): $(OBJ_DIR) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo -e "\e[32mLinking complete: $(KERNEL_BIN)\e[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.asm
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
