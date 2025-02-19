// src/parser.cpp
#include "parser.h"
#include <iostream>
#include <system_error>

Parser::Parser(const std::string& filename) 
    : filename_(filename), fileHandle_(INVALID_HANDLE_VALUE) {}

Parser::~Parser() {
    if (fileHandle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(fileHandle_);
    }
}

bool Parser::parse() {
    // Open the file
    fileHandle_ = CreateFileA(filename_.c_str(), GENERIC_READ, FILE_SHARE_READ, 
                            nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle_ == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening file: " << filename_ << std::endl;
        return false;
    }

    // Read DOS header
    DWORD bytesRead;
    if (!ReadFile(fileHandle_, &dosHeader_, sizeof(IMAGE_DOS_HEADER), &bytesRead, nullptr) ||
        bytesRead != sizeof(IMAGE_DOS_HEADER)) {
        std::cerr << "Error reading DOS header" << std::endl;
        return false;
    }

    // Check if it's a valid PE file
    if (dosHeader_.e_magic != IMAGE_DOS_SIGNATURE) {
        std::cerr << "Error: Not a valid PE file" << std::endl;
        return false;
    }

    // Seek to PE header
    LARGE_INTEGER moveDistance;
    moveDistance.QuadPart = dosHeader_.e_lfanew;
    if (!SetFilePointerEx(fileHandle_, moveDistance, nullptr, FILE_BEGIN)) {
        std::cerr << "Error seeking to PE header" << std::endl;
        return false;
    }

    // Read PE header
    if (!ReadFile(fileHandle_, &ntHeaders_, sizeof(IMAGE_NT_HEADERS), &bytesRead, nullptr) ||
        bytesRead != sizeof(IMAGE_NT_HEADERS)) {
        std::cerr << "Error reading PE header" << std::endl;
        return false;
    }

    // Check PE signature
    if (ntHeaders_.Signature != IMAGE_NT_SIGNATURE) {
        std::cerr << "Error: Invalid PE signature" << std::endl;
        return false;
    }

    // Find and read .text section
    IMAGE_SECTION_HEADER sectionHeader;
    for (int i = 0; i < ntHeaders_.FileHeader.NumberOfSections; i++) {
        if (!ReadFile(fileHandle_, &sectionHeader, sizeof(IMAGE_SECTION_HEADER), &bytesRead, nullptr) ||
            bytesRead != sizeof(IMAGE_SECTION_HEADER)) {
            std::cerr << "Error reading section header" << std::endl;
            return false;
        }

        if (memcmp(sectionHeader.Name, ".text", 5) == 0) {
            // Found .text section, read its contents
            textSection_.resize(sectionHeader.SizeOfRawData);
            
            moveDistance.QuadPart = sectionHeader.PointerToRawData;
            if (!SetFilePointerEx(fileHandle_, moveDistance, nullptr, FILE_BEGIN)) {
                std::cerr << "Error seeking to .text section" << std::endl;
                return false;
            }

            if (!ReadFile(fileHandle_, textSection_.data(), sectionHeader.SizeOfRawData, 
                         &bytesRead, nullptr) || bytesRead != sectionHeader.SizeOfRawData) {
                std::cerr << "Error reading .text section" << std::endl;
                return false;
            }
            return true;
        }
    }

    std::cerr << "Error: .text section not found" << std::endl;
    return false;
}

std::vector<uint8_t> Parser::getTextSection() {
    return textSection_;
}