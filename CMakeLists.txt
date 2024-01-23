cmake_minimum_required(VERSION 3.22)
project("Fantasy Computer")

add_executable("fantacom"
    "src/main.cpp"
    "src/globals.cpp"
    "src/machine.cpp"

    "src/io_devices/mmu_controller.cpp"
)
target_include_directories("fantacom" PRIVATE "include")


# "Package maintainers are encouraged to use at least the following options for the shared library"
# I'll take that as "use these options for the most accurate emulation"
set(Z80_WITH_EXECUTE YES)
set(Z80_WITH_FULL_IM0 YES)
set(Z80_WITH_IM0_RETX_NOTIFICATIONS YES)
set(Z80_WITH_Q YES)
set(Z80_WITH_ZILOG_NMOS_LD_A_IR_BUG YES)

set(Z80_SHARED_LIBS NO)
add_subdirectory("3rd/Z80")

add_subdirectory("3rd/argparse")
add_subdirectory("3rd/raylib")

target_link_libraries("fantacom" "Z80" "argparse" "raylib")
