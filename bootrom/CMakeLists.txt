cmake_minimum_required(VERSION 3.22)
project(bootrom ASM)
set(CMAKE_TOOLCHAIN_FILE binutils-z80.cmake)
configure_file(link.ld link.ld)

add_executable(bootrom
    src/reset.s
    src/rt0.s
    src/main.s
)
target_link_options(bootrom PRIVATE -T link.ld)

add_custom_target(bootrom.bin ALL
    COMMAND ${CMAKE_OBJCOPY} -O binary bootrom bootrom.bin
    DEPENDS bootrom
    BYPRODUCTS bootrom.bin
)
