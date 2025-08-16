cmake_minimum_required(VERSION 3.22)
project("Fantasy Computer")
add_subdirectory(3rd)

add_executable(fantacom
    src/main.cpp
    src/z80.cpp
    src/board.cpp
)
target_include_directories(fantacom PRIVATE include)
target_link_libraries(fantacom PRIVATE argparse Z80)
