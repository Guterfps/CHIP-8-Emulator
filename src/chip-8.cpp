
#include <fstream>
#include <iterator>
#include <iostream>

#include "chip-8.hpp"

namespace Emulation
{

CHIP_8::CHIP_8()
{   
    for (std::size_t i = 0; i < NUM_OF_FONTS; ++i) {
        m_memory[i] = s_font_set[i];
    }
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
            m_opcodes_table[last_4_bits](opcode);
        } else {
            std::cout << "unknown opcode [0x0000]: " << opcode << '\n';
        } 
    } else if (m_opcodes_table.end() != m_opcodes_table.find(first_4_bits)) {
        m_opcodes_table[first_4_bits](opcode);
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

} // Emulation
