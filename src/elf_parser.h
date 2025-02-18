// src/elf_parser.h
#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <string>
#include <vector>
#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>

class ElfParser {
public:
    ElfParser(const std::string& filename);
    ~ElfParser();
    
    bool parse();
    // Get the .text section (contains code)
    std::vector<uint8_t> getTextSection();

private:
    std::string filename_;
    int fd_;                 // File descriptor
    Elf* elf_;              // ELF descriptor
    std::vector<uint8_t> textSection_;
};

#endif