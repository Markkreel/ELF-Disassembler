cmake_minimum_required(VERSION 3.10)
project(disassembler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libelf
find_library(LIBELF_LIBRARY elf REQUIRED)

add_executable(disassembler
    src/main.cpp
    src/elf_parser.cpp
)

target_link_libraries(disassembler PRIVATE ${LIBELF_LIBRARY})