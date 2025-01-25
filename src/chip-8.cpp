
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "chip-8.hpp"

namespace Emulator
{

CHIP_8::CHIP_8()
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
    m_opcode = (m_memory[m_pc] << 8) | m_memory[m_pc + 1];
    
    uint16_t first_4_bits = m_opcode & 0xF000;
    if (0x0000 == first_4_bits) {
        uint16_t last_4_bits = m_opcode & 0x000F;
        if (s_opcode_tabale.end() != s_opcode_tabale.find(last_4_bits)) {
            s_opcode_tabale.at(last_4_bits)(this);
        } else {
            std::cout << "unknown opcode [0x0000]: " << m_opcode << '\n';
        } 
    } else if (s_opcode_tabale.end() != s_opcode_tabale.find(first_4_bits)) {
        s_opcode_tabale.at(first_4_bits)(this);
    } else {
        std::cout << "unknown opcode: " << m_opcode << '\n';
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

void CHIP_8::Op0x1NNN(CHIP_8 *chip) {
    constexpr uint16_t ADDR_NNN = 0x0FFF;

    chip->m_pc = chip->m_opcode & ADDR_NNN; 
}

void CHIP_8::Op0x2NNN(CHIP_8 *chip) {
    constexpr uint16_t ADDR_NNN = 0x0FFF;

    chip->m_stack.Push(chip->m_pc);
    chip->m_pc = chip->m_opcode & ADDR_NNN;
}

void CHIP_8::Op0x00EE(CHIP_8 *chip) {
   chip->m_pc = chip->m_stack.Pop();
   chip->m_pc += 2;
}

void CHIP_8::Op0x3XNN(CHIP_8 *chip) {
    constexpr uint16_t VAL_NN = 0x00FF;
    constexpr uint16_t REG_X = 0x0F00;

    uint16_t amount_to_move = 2;

    if ((chip->m_opcode & VAL_NN) == (chip->m_regs_V[chip->m_opcode & REG_X])) {
        amount_to_move += 2;
    }
    
    chip->m_pc += amount_to_move;
}

const std::unordered_map<uint16_t, std::function<void(CHIP_8*)>>  CHIP_8::s_opcode_tabale =
{
    // flow operations
    {0x1000, &Op0x1NNN},
    {0x2000, &Op0x2NNN},
    {0x00EE, &Op0x00EE},
    
    {0x3000, &Op0x3XNN}
    
};

const std::array<uint8_t, CHIP_8::NUM_OF_FONTS> CHIP_8::s_font_set =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

} // Emulation
