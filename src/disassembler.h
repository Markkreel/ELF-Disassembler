// src/disassembler.h
#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <capstone/capstone.h>

class Disassembler {
public:
    // Constructor and destructor
    Disassembler();
    ~Disassembler();

    // Initialize the disassembler with binary code
    bool init(const std::vector<uint8_t>& code, uint64_t address = 0);

    // Disassemble the entire binary
    bool disassembleAll();

    // Save complete disassembly to a file
    bool saveDisassembly(const std::string& filename) const;

    // Save code blocks to a file
    bool saveCodeBlocks(const std::string& filename) const;

private:
    // Structure to represent a code block
    struct CodeBlock {
        uint64_t startAddress;
        uint64_t endAddress;
        std::vector<cs_insn> instructions;
    };

    // Find and analyze code blocks
    void analyzeCodeBlocks();

    // Check if instruction is a control flow instruction
    bool isControlFlowInstruction(const cs_insn& instruction) const;

    // Capstone engine handle
    csh handle_;
    // Binary code to disassemble
    std::vector<uint8_t> code_;
    // Base address for the code
    uint64_t baseAddress_;
    // Disassembled instructions
    std::vector<cs_insn> instructions_;
    // Code blocks
    std::vector<CodeBlock> codeBlocks_;
    // Initialization status
    bool initialized_;
};