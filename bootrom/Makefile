# Compiler, assembler, and linker
CC = zcc
AS = z88dk-z80asm

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SRC_DIR = src

CFLAGS = +z80 -clib=classic -I$(SRC_DIR)/include -c
CRT_OPTS = CRT_ORG_BSS=0x2000 REGISTER_SP=0x3000 CRT_HEAP_ENABLE=3
LFLAGS = +z80 -clib=classic -crt0=crt0.asm $(addprefix -pragma-define:, $(CRT_OPTS)) -create-app -m

.PHONY: configure build clean

configure:
	@echo "Configuring build environment..."
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning build environment..."
	@find $(BUILD_DIR) -type f -delete

build: main.o z80io.o console.o keyboard.o ivt.o disk.o
	@echo "Linking..."
	@$(CC) $(LFLAGS) -o$(BUILD_DIR)/bootrom $(patsubst %.o, $(OBJ_DIR)/%.o, $^)

%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -o$(OBJ_DIR)/$@ $<

%.o: $(SRC_DIR)/%.asm
	@echo "Assembling $<..."
	@$(AS) -o$(OBJ_DIR)/$@ $<
