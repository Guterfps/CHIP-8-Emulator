
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

inline void CHIP_8::MoveToNextOp(uint16_t *pc) {
    constexpr uint16_t SIZE_OF_OPCODE = 2;
    
    *pc += SIZE_OF_OPCODE;
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
   MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x3XNN(CHIP_8 *chip) {
    constexpr uint16_t VAL_NN = 0x00FF;
    constexpr uint16_t REG_X = 0x0F00;

    uint16_t opcode = chip->m_opcode;
    uint16_t amount_to_move = 2;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    if ((opcode & VAL_NN) ==
        (chip->m_regs_V[regx_indx])) {
        amount_to_move += 2;
    }
    
    chip->m_pc += amount_to_move;
}

void CHIP_8::Op0x4XNN(CHIP_8 *chip) {
    constexpr uint16_t VAL_NN = 0x00FF;
    constexpr uint16_t REG_X = 0x0F00;

    uint16_t opcode = chip->m_opcode;
    uint16_t amount_to_move = 2;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    if ((opcode & VAL_NN) !=
        (chip->m_regs_V[regx_indx])) {
        amount_to_move += 2;
    }
    
    chip->m_pc += amount_to_move;
}

void CHIP_8::Op0x5XY0(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    uint16_t amount_to_move = 2;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4; 

    if ((chip->m_regs_V[regx_indx]) ==
        (chip->m_regs_V[regy_indx])) {
        amount_to_move += 2;
    }
    
    chip->m_pc += amount_to_move;
}

void CHIP_8::Op0x6XNN(CHIP_8 *chip) {
    constexpr uint16_t VAL_NN = 0x00FF;
    constexpr uint16_t REG_X = 0x0F00;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[regx_indx] = opcode & VAL_NN;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x7XNN(CHIP_8 *chip) {
    constexpr uint16_t VAL_NN = 0x00FF;
    constexpr uint16_t REG_X = 0x0F00;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[regx_indx] += opcode & VAL_NN;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY0(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4; 

    chip->m_regs_V[regx_indx] = chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY1(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[regx_indx] |= chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY2(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[regx_indx] &= chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY3(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[regx_indx] ^= chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY4(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[0x0F] = (chip->m_regs_V[regx_indx] >
                            (0xFF - chip->m_regs_V[regy_indx])) ?
                            1 : 0;

    chip->m_regs_V[regx_indx] += chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY5(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[0x0F] = (chip->m_regs_V[regx_indx] <
                            chip->m_regs_V[regy_indx]) ?
                            0 : 1;

    chip->m_regs_V[regx_indx] -= chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY6(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint8_t LSB = 1;
    
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> LSB;

    chip->m_regs_V[0x0F] = chip->m_regs_V[regx_indx] & 1;

    chip->m_regs_V[regx_indx] >>= 1;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY7(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint16_t REG_Y = 0x00F0;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[0x0F] = (chip->m_regs_V[regx_indx] >
                            chip->m_regs_V[regy_indx]) ?
                            0 : 1;
    
    chip->m_regs_V[regx_indx] = chip->m_regs_V[regy_indx] - chip->m_regs_V[regx_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XYE(CHIP_8 *chip) {
    constexpr uint16_t REG_X = 0x0F00;
    constexpr uint8_t MSB = 1 << 7;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[0x0F] = chip->m_regs_V[regx_indx] & MSB;

    chip->m_regs_V[regx_indx] <<= 1;

    MoveToNextOp(&(chip->m_pc));
}


const std::unordered_map<uint16_t, std::function<void(CHIP_8*)>>  CHIP_8::s_opcode_tabale =
{
    // Flow operations
    {0x1000, &Op0x1NNN},
    {0x2000, &Op0x2NNN},
    {0x000E, &Op0x00EE},

    // Cond operations 
    {0x3000, &Op0x3XNN},
    {0x4000, &Op0x4XNN},
    {0x5000, &Op0x5XY0},

    // Const operations
    {0x6000, &Op0x6XNN},
    {0x7000, &Op0x7XNN},

    // Assig operations
    {0x8000, &Op0x8XY0},

    // BitOp
    {0x8001, &Op0x8XY1},
    {0x8002, &Op0x8XY2},
    {0x8003, &Op0x8XY3},
    {0x8006, &Op0x8XY6},
    {0x800E, &Op0x8XYE},

    // Math operations
    {0x8004, &Op0x8XY4},
    {0x8005, &Op0x8XY5},
    {0x8007, &Op0x8XY7},
    
    
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
