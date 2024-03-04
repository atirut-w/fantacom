# Compiler, assembler, and linker
CC = zcc
AS = z88dk-z80asm

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SRC_DIR = src

CFLAGS = +z80 -clib=classic -I$(SRC_DIR)/include -c
LFLAGS = +z80 -clib=classic -crt0=crt0.asm -m

SRCS = $(wildcard $(SRC_DIR)/*.c)

.PHONY: configure build clean

configure:
	@echo "Configuring build environment..."
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning build environment..."
	@find $(BUILD_DIR) -type f -delete

build: main.o z80io.o display.o
	@echo "Linking..."
	@$(CC) $(LFLAGS) -o$(BUILD_DIR)/bootrom.bin $(patsubst %.o, $(OBJ_DIR)/%.o, $^)

%.o: $(SRC_DIR)/%.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -o$(OBJ_DIR)/$@ $<

%.o: $(SRC_DIR)/%.asm
	@echo "Assembling $@..."
	@$(AS) -o$(OBJ_DIR)/$@ $<
