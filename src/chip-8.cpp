
#include <fstream>
#include <iterator>
#include <iostream>

#include "chip-8.hpp"

namespace Emulator
{

CHIP_8::CHIP_8() : m_opcodes_table(InitOpcodesTable())
{   
    LoadFonts(m_memory);
}

CHIP_8::Status CHIP_8::LoadBin(const char *file_path) {
    std::ifstream input(file_path, std::ios::binary);
    if (!input.is_open()) {
        std::cout << "faild to open: " << file_path << '\n';

        return Status::ERROR;
    }

    input.read(reinterpret_cast<char*>(&m_memory[PROGRAM_START_ADDR]), MEMORY_SIZE - PROGRAM_START_ADDR);
    if (!input.good()) {
        std::cout << "faild to read the file\n";

        return Status::ERROR;
    }

    return Status::SUCESS;
}

void CHIP_8::EmulateCycle() {
    uint16_t opcode = m_memory[m_pc] << 8 | m_memory[m_pc + 1];
    
    uint16_t first_4_bits = opcode & 0xF000;
    if (0x0000 == first_4_bits) {
        uint16_t last_4_bits = opcode & 0x000F;
        if (m_opcodes_table.end() != m_opcodes_table.find(last_4_bits)) {
            m_opcodes_table[last_4_bits](this);
        } else {
            std::cout << "unknown opcode [0x0000]: " << opcode << '\n';
        } 
    } else if (m_opcodes_table.end() != m_opcodes_table.find(first_4_bits)) {
        m_opcodes_table[first_4_bits](this);
    } else {
        std::cout << "unknown opcode: " << opcode << '\n';
    }

    if (m_delay_timer > 0) {
        --m_delay_timer;
    }
    
    if (m_sound_timer > 0) {
        if (0 == m_sound_timer) {
            std::cout << "BEEP!\n";
        }
        --m_sound_timer;
    }
}

inline void CHIP_8::LoadFonts(std::array<unsigned char, MEMORY_SIZE>& memory) {
    for (std::size_t i = 0; i < NUM_OF_FONTS; ++i) {
        memory[i] = s_font_set[i];
    }
}

inline std::unordered_map<uint16_t, std::function<void(CHIP_8*)>> CHIP_8::InitOpcodesTable() {
    std::unordered_map<uint16_t, std::function<void(CHIP_8*)>> opcodes_table = 
    {
        {0x200, &Op0x200},
    }; 

    return opcodes_table;
}

void CHIP_8::Op0x200(CHIP_8 *chip) {
    constexpr uint16_t ADDR_NNN = 0x0FFF;

    chip->m_stack[chip->m_stack.GetSP()] = chip->m_pc;
    chip->m_stack.MoveSP(1);
    chip->m_pc = chip->m_opcode & ADDR_NNN;
}

} // Emulation
