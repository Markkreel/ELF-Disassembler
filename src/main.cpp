// src/main.cpp
#include <iostream>
#include "elf_parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <elf_file>" << std::endl;
        return 1;
    }

    try {
        ElfParser parser(argv[1]);
        
        if (!parser.parse()) {
            std::cerr << "Failed to parse ELF file" << std::endl;
            return 1;
        }

        // Get the .text section (contains the code)
        auto textSection = parser.getTextSection();
        std::cout << "Successfully read .text section, size: " 
                  << textSection.size() << " bytes" << std::endl;

        // Here we'll later add disassembly code
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}