add_subdirectory(argparse)

add_library(chips INTERFACE)
target_include_directories(chips INTERFACE chips)
