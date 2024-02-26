# Compiler, assembler, and linker
CC = zcc
AS = z88dk-z80asm

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/o
SRC_DIR = src

CFLAGS = +z80 -clib=classic -I$(SRC_DIR)/include

.PHONY: configure clean
runtime: runtime/l_gchar.o

configure:
	@echo "Configuring build environment..."
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

build: crt0.o main.o z80io.o display.o
	@echo "Linking bootrom..."
	@$(AS) -b -o$(BUILD_DIR)/bootrom.bin $(OBJ_DIR)/*.o

clean:
	@echo "Cleaning build environment..."
	@find $(BUILD_DIR) -type f -delete

%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -o $(OBJ_DIR)/$@ -c $<

%.o: $(SRC_DIR)/%.asm
	@echo "Assembling $<..."
	@$(AS) -o$(OBJ_DIR)/$@ $<
