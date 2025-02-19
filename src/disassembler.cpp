// src/disassembler.cpp
#include "disassembler.h"
#include <iostream>
#include <fstream>
#include <algorithm>

Disassembler::Disassembler() : handle_(0), baseAddress_(0), initialized_(false) {}

Disassembler::~Disassembler() {
    if (initialized_) {
        cs_close(&handle_);
    }
}

bool Disassembler::init(const std::vector<uint8_t>& code, uint64_t address) {
    // Initialize Capstone
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle_) != CS_ERR_OK) {
        std::cerr << "Failed to initialize Capstone engine" << std::endl;
        return false;
    }

    // Enable detailed instruction information
    cs_option(handle_, CS_OPT_DETAIL, CS_OPT_ON);

    code_ = code;
    baseAddress_ = address;
    initialized_ = true;
    return true;
}

bool Disassembler::disassembleAll() {
    if (!initialized_) return false;

    // Clear previous results
    instructions_.clear();
    codeBlocks_.clear();

    // Disassemble all instructions
    size_t count;
    cs_insn* insn = cs_malloc(handle_);
    
    const uint8_t* code_ptr = code_.data();
    size_t code_size = code_.size();
    uint64_t address = baseAddress_;

    while (cs_disasm_iter(handle_, &code_ptr, &code_size, &address, insn)) {
        instructions_.push_back(*insn);
    }

    cs_free(insn, 1);

    // Analyze code blocks after disassembly
    if (!instructions_.empty()) {
        analyzeCodeBlocks();
        return true;
    }

    return false;
}

void Disassembler::analyzeCodeBlocks() {
    if (instructions_.empty()) return;

    CodeBlock currentBlock;
    currentBlock.startAddress = instructions_[0].address;
    currentBlock.instructions.push_back(instructions_[0]);

    for (size_t i = 1; i < instructions_.size(); ++i) {
        const auto& prevInst = instructions_[i - 1];
        const auto& currInst = instructions_[i];

        // Add instruction to current block
        currentBlock.instructions.push_back(currInst);

        // Check if previous instruction ends a block
        if (isControlFlowInstruction(prevInst)) {
            currentBlock.endAddress = prevInst.address;
            codeBlocks_.push_back(currentBlock);

            // Start new block
            currentBlock = CodeBlock();
            currentBlock.startAddress = currInst.address;
            currentBlock.instructions.clear();
            currentBlock.instructions.push_back(currInst);
        }
    }

    // Add the last block if not empty
    if (!currentBlock.instructions.empty()) {
        currentBlock.endAddress = currentBlock.instructions.back().address;
        codeBlocks_.push_back(currentBlock);
    }
}

bool Disassembler::isControlFlowInstruction(const cs_insn& instruction) const {
    // Check for common control flow instructions
    switch (instruction.id) {
        case X86_INS_JMP:
        case X86_INS_CALL:
        case X86_INS_RET:
        case X86_INS_JE:
        case X86_INS_JNE:
        case X86_INS_JA:
        case X86_INS_JAE:
        case X86_INS_JB:
        case X86_INS_JBE:
        case X86_INS_JG:
        case X86_INS_JGE:
        case X86_INS_JL:
        case X86_INS_JLE:
            return true;
        default:
            return false;
    }
}

bool Disassembler::saveDisassembly(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) return false;

    for (const auto& instruction : instructions_) {
        file << std::hex << "0x" << instruction.address << ":\t"
             << instruction.mnemonic << " "
             << instruction.op_str << std::endl;
    }

    return true;
}

bool Disassembler::saveCodeBlocks(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) return false;

    for (size_t i = 0; i < codeBlocks_.size(); ++i) {
        const auto& block = codeBlocks_[i];
        file << "Block " << i << " (0x" << std::hex << block.startAddress
             << " - 0x" << block.endAddress << "):\n";

        for (const auto& instruction : block.instructions) {
            file << "  0x" << instruction.address << ":\t"
                 << instruction.mnemonic << " "
                 << instruction.op_str << std::endl;
        }
        file << std::endl;
    }

    return true;
}