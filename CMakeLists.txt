cmake_minimum_required(VERSION 3.10)
project(disassembler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Capstone
find_path(CAPSTONE_INCLUDE_DIR
    NAMES capstone/capstone.h
    PATH_SUFFIXES include
    REQUIRED
)

find_library(CAPSTONE_LIBRARY
    NAMES capstone
    REQUIRED
)

add_executable(disassembler
    src/main.cpp
    src/parser.cpp
    src/disassembler.cpp
    src/output.cpp
)

# Include directories
target_include_directories(disassembler PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CAPSTONE_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(disassembler PRIVATE
    ${CAPSTONE_LIBRARY}
)