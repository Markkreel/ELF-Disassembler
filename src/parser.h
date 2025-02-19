// src/parser.h
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <windows.h>

class Parser {
public:
    Parser(const std::string& filename);
    ~Parser();
    
    bool parse();
    // Get the .text section (contains code)
    std::vector<uint8_t> getTextSection();

private:
    std::string filename_;
    HANDLE fileHandle_;
    std::vector<uint8_t> textSection_;
    
    // PE specific members
    IMAGE_DOS_HEADER dosHeader_;
    IMAGE_NT_HEADERS ntHeaders_;
};

#endif