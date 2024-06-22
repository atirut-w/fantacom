set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_ASM_COMPILER z80-elf-as CACHE PATH "Z80 assembler")
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <FLAGS> -o <OBJECT> <SOURCE>")
