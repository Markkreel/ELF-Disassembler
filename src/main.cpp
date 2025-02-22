// src/main.cpp
#include <iostream>
#include "elf_parser.h"
#include "disassembler.h"

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

        // Initialize disassembler
        Disassembler disasm;
        if (!disasm.init(textSection)) {
            std::cerr << "Failed to initialize disassembler" << std::endl;
            return 1;
        }

        // Perform disassembly
        if (!disasm.disassembleAll()) {
            std::cerr << "Failed to disassemble code" << std::endl;
            return 1;
        }

        // Save output files
        if (!disasm.saveDisassembly("disassembly.txt")) {
            std::cerr << "Failed to save disassembly" << std::endl;
            return 1;
        }

        if (!disasm.saveCodeBlocks("blocks.txt")) {
            std::cerr << "Failed to save code blocks" << std::endl;
            return 1;
        }

        std::cout << "Disassembly complete. Output written to disassembly.txt and blocks.txt" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}