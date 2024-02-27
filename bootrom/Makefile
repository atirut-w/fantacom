# Compiler, assembler, and linker
CC = zcc
AS = z88dk-z80asm

BUILD_DIR = build
SRC_DIR = src

CFLAGS = +z80 -clib=classic -I$(SRC_DIR)/include -crt0=crt0.asm

SRCS = $(wildcard $(SRC_DIR)/*.c)

.PHONY: configure build clean

configure:
	@echo "Configuring build environment..."
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "Cleaning build environment..."
	@find $(BUILD_DIR) -type f -delete

build: src
	@echo "Building bootrom..."
	@$(CC) $(CFLAGS) -o$(BUILD_DIR)/bootrom.bin $(SRCS) -create-app -m
