// src/elf_parser.cpp
#include "elf_parser.h"
#include <iostream>
#include <system_error>

ElfParser::ElfParser(const std::string& filename) 
    : filename_(filename), fd_(-1), elf_(nullptr) {
    // Initialize ELF library
    if (elf_version(EV_CURRENT) == EV_NONE) {
        throw std::runtime_error("Failed to initialize libelf");
    }
}

ElfParser::~ElfParser() {
    if (elf_) elf_end(elf_);
    if (fd_ != -1) close(fd_);
}

bool ElfParser::parse() {
    // Open the file
    fd_ = open(filename_.c_str(), O_RDONLY);
    if (fd_ == -1) {
        std::cerr << "Error opening file: " << filename_ << std::endl;
        return false;
    }

    // Create ELF descriptor
    elf_ = elf_begin(fd_, ELF_C_READ, nullptr);
    if (!elf_) {
        std::cerr << "Error: " << elf_errmsg(-1) << std::endl;
        return false;
    }

    // Check if it's really an ELF file
    if (elf_kind(elf_) != ELF_K_ELF) {
        std::cerr << "Error: Not an ELF file" << std::endl;
        return false;
    }

    // Get the .text section
    Elf_Scn* scn = nullptr;
    GElf_Shdr shdr;
    char* name;

    // Get section header string table index
    size_t shstrndx;
    if (elf_getshdrstrndx(elf_, &shstrndx) != 0) {
        std::cerr << "Error getting section header string table index" << std::endl;
        return false;
    }

    // Find the .text section
    while ((scn = elf_nextscn(elf_, scn)) != nullptr) {
        if (gelf_getshdr(scn, &shdr) != &shdr) {
            continue;
        }

        name = elf_strptr(elf_, shstrndx, shdr.sh_name);
        if (name && strcmp(name, ".text") == 0) {
            // Found .text section, get its data
            Elf_Data* data = elf_getdata(scn, nullptr);
            if (data) {
                textSection_.resize(data->d_size);
                memcpy(textSection_.data(), data->d_buf, data->d_size);
                return true;
            }
        }
    }

    return false;
}

std::vector<uint8_t> ElfParser::getTextSection() {
    return textSection_;
}