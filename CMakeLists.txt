cmake_minimum_required(VERSION 3.22)
project("Fantasy Computer")
add_subdirectory(3rd)

add_executable(fantacom
    src/main.cpp
)
target_link_libraries(fantacom PRIVATE argparse)
