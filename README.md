# ELF Parser & Disassembler

A simple ELF file disassembler that reads executable files and converts machine code into human-readable assembly code.

## Project Status

Currently in early development. The project can:

- Parse ELF files
- Extract the .text section (which contains the program code)

## Prerequisites

- C++ compiler with C++17 support
- CMake (version 3.10 or higher)
- libelf development package

### Installing Prerequisites

On Ubuntu/Debian:

```bash
sudo apt-get install build-essential cmake libelf-dev
```

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
./disassembler <elf_file>
```

### Project Structure

```
disassembler/
├── src/                    # Source code files
│   ├── main.cpp           # Main entry point
│   ├── elf_parser.cpp     # ELF file parsing code
│   ├── elf_parser.h       # Header for ELF parser
├── include/               # External library headers
├── lib/                   # External libraries
├── build/                 # Compiled binaries
├── test/                  # Test files
│   └── test_files/       # Sample ELF files for testing
├── CMakeLists.txt        # Build configuration
└── README.md             # Project documentation
```
