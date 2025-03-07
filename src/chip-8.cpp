
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <random>

#include "chip-8.hpp"
#include "key_pad.hpp"

namespace Emulator
{

CHIP_8::CHIP_8() : m_time(std::chrono::steady_clock::now())
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
    if (!input.fail()) {
        std::cout << "faild to read the file: " << input.gcount() << '\n';

        return Status::ERROR;
    }

    return Status::SUCESS;
}

void CHIP_8::EmulateCycle() {
    m_opcode = (m_memory[m_pc] << 8) | m_memory[m_pc + 1];

    uint16_t table_index = FindOpcodeTableIndex(m_opcode);
    const auto table_iter = s_opcode_tabale.find(table_index);

    if (table_iter != s_opcode_tabale.end()) {
        table_iter->second(this);
        std::cout <<"opcode: " << std::hex << table_index << ", " << std::hex << m_opcode << '\n';
    } else {
        std::cout << "unknown opcode: "<< std::hex << m_opcode << '\n';
    }

    if (IsTimerTick()) {
        TickDelayTimer();
        TickSoundTimer();
    }
}

void CHIP_8::SetKey(KeyPad::Keys key, bool pressed) {
    m_key_pad.SetKey(key, pressed);
}

inline uint16_t CHIP_8::FindOpcodeTableIndex(uint16_t opcode) {
    constexpr uint16_t FIRST_4_BITS = 0xF000;
    constexpr uint16_t LAST_4_BITS = 0x000F;
    constexpr uint16_t LAST_8_BITS = 0x00FF;
    
    uint16_t table_index = opcode & FIRST_4_BITS;

    switch (table_index)
    {
        case 0x0000:
            table_index |= opcode & LAST_8_BITS;
            break;
        case 0x8000:
            table_index |= opcode & LAST_4_BITS;
            break;
        case 0xE000:
            table_index |= opcode & LAST_8_BITS;
            break;
        case 0xF000:
            table_index |= opcode & LAST_8_BITS;
            break;
        default:
            break;
    }

    return table_index;
}

inline void CHIP_8::LoadFonts(std::array<unsigned char, MEMORY_SIZE>& memory) {
    for (std::size_t i = 0; i < NUM_OF_FONTS; ++i) {
        memory[i] = s_font_set[i];
    }
}

inline void CHIP_8::MoveToNextOp(uint16_t *pc) {
    *pc += SIZE_OF_OPCODE;
}

inline void CHIP_8::TickDelayTimer() {
    if (m_delay_timer > 0) {
        --m_delay_timer;
    }
}

inline void CHIP_8::TickSoundTimer() {
    if (m_sound_timer > 0) {
        m_sound_flag = true;
        --m_sound_timer;
    }
}

inline bool CHIP_8::IsTimerTick() {
    return (SecondsPassed() >= TIMERS_SPEED_IN_SEC);
}

inline double CHIP_8::SecondsPassed() {
    std::chrono::time_point<std::chrono::steady_clock> cur_time{std::chrono::steady_clock::now()};
    std::chrono::duration<double> elapsed_secs{cur_time - m_time};

    return elapsed_secs.count();
}

void CHIP_8::Op0x1NNN(CHIP_8 *chip) {
    chip->m_pc = chip->m_opcode & ADDR_NNN; 
}

void CHIP_8::Op0x2NNN(CHIP_8 *chip) {
    chip->m_stack.Push(chip->m_pc);
    chip->m_pc = chip->m_opcode & ADDR_NNN;
}

void CHIP_8::Op0x00EE(CHIP_8 *chip) {
   chip->m_pc = chip->m_stack.Pop();
   MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x3XNN(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    uint16_t amount_to_move = SIZE_OF_OPCODE;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    if ((opcode & VAL_NN) ==
        (chip->m_regs_V[regx_indx])) {
        amount_to_move += SIZE_OF_OPCODE;
    }
    
    chip->m_pc += amount_to_move;
}

void CHIP_8::Op0x4XNN(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    uint16_t amount_to_move = SIZE_OF_OPCODE;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    if ((opcode & VAL_NN) !=
        (chip->m_regs_V[regx_indx])) {
        amount_to_move += SIZE_OF_OPCODE;
    }
    
    chip->m_pc += amount_to_move;
}

void CHIP_8::Op0x5XY0(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    uint16_t amount_to_move = SIZE_OF_OPCODE;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4; 

    if ((chip->m_regs_V[regx_indx]) ==
        (chip->m_regs_V[regy_indx])) {
        amount_to_move += SIZE_OF_OPCODE;
    }
    
    chip->m_pc += amount_to_move;
}

void CHIP_8::Op0x6XNN(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[regx_indx] = opcode & VAL_NN;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x7XNN(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[regx_indx] += opcode & VAL_NN;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY0(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4; 

    chip->m_regs_V[regx_indx] = chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY1(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[regx_indx] |= chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY2(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[regx_indx] &= chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY3(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;

    chip->m_regs_V[regx_indx] ^= chip->m_regs_V[regy_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY4(CHIP_8 *chip) {
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
    constexpr uint8_t LSB = 1;
    
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[0x0F] = chip->m_regs_V[regx_indx] & LSB;

    chip->m_regs_V[regx_indx] >>= 1;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x8XY7(CHIP_8 *chip) {
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
    constexpr uint8_t MSB = 1 << 7;

    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[0x0F] = (chip->m_regs_V[regx_indx] & MSB) >> 7;

    chip->m_regs_V[regx_indx] <<= 1;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0x9XY0(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;
    uint16_t amount_to_move = SIZE_OF_OPCODE;

    if (chip->m_regs_V[regx_indx] !=
        chip->m_regs_V[regy_indx]) {
        amount_to_move += SIZE_OF_OPCODE;
    }
    
    chip->m_pc += amount_to_move;
}

void CHIP_8::Op0xANNN(CHIP_8 *chip) {
    chip->m_index_reg = chip->m_opcode & ADDR_NNN;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xBNNN(CHIP_8 *chip) {
    chip->m_pc = chip->m_regs_V[0] + (chip->m_opcode & ADDR_NNN);
}

void CHIP_8::Op0xCXNN(CHIP_8 *chip) {
    const uint16_t opcode = chip->m_opcode;
    const uint16_t regx_indx = (opcode & REG_X) >> 8;

    chip->m_regs_V[regx_indx] = RandomNumber() & (opcode & VAL_NN);

    MoveToNextOp(&(chip->m_pc));
}

inline uint8_t CHIP_8::RandomNumber() {
    static std::random_device dev;
    static std::mt19937 gen(dev());
    static std::uniform_int_distribution<uint16_t> distrib(0, 255);

    return distrib(gen);
}

void CHIP_8::Op0xFX07(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;

    chip->m_regs_V[regx_indx] = chip->m_delay_timer;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xFX15(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;

    chip->m_delay_timer = chip->m_regs_V[regx_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xFX18(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;

    chip->m_sound_timer = chip->m_regs_V[regx_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xFX1E(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;

    chip->m_index_reg += chip->m_regs_V[regx_indx];

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xFX55(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;
    const uint16_t I = chip->m_index_reg;
    
    for (uint16_t i = 0; i <= regx_indx; ++i) {
        chip->m_memory[I + i] = chip->m_regs_V[i];
    }

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xFX65(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;
    const uint16_t I = chip->m_index_reg;

    for (uint16_t i = 0; i <= regx_indx; ++i) {
        chip->m_regs_V[i] = chip->m_memory[I + i];
    }

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xFX33(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;
    const uint16_t I = chip->m_index_reg;

    chip->m_memory[I] = chip->m_regs_V[regx_indx] / 100;
    chip->m_memory[I + 1] = (chip->m_regs_V[regx_indx] / 10) % 10;
    chip->m_memory[I + 2] = (chip->m_regs_V[regx_indx] % 100) % 10;
    
    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xFX29(CHIP_8 *chip) {
    constexpr uint8_t FONT_SIZE = 5;

    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;

    chip->m_index_reg = chip->m_regs_V[regx_indx] * FONT_SIZE;

    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xEX9E(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;
    uint16_t amount_to_move = SIZE_OF_OPCODE;

    if (chip->m_key_pad.IsKeyPressed(
        static_cast<KeyPad::Keys>(chip->m_regs_V[regx_indx] & 0x0F))) {
        amount_to_move += SIZE_OF_OPCODE;
    }

    chip->m_pc += amount_to_move;    
}

void CHIP_8::Op0xEXA1(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;
    uint16_t amount_to_move = SIZE_OF_OPCODE;

    if (!chip->m_key_pad.IsKeyPressed(
        static_cast<KeyPad::Keys>(chip->m_regs_V[regx_indx] & 0x0F))) {
        amount_to_move += SIZE_OF_OPCODE;
    }

    chip->m_pc += amount_to_move;    
}

void CHIP_8::Op0xFX0A(CHIP_8 *chip) {
    const uint16_t regx_indx = (chip->m_opcode & REG_X) >> 8;

    const KeyPad::Keys key_pressed = chip->m_key_pad.GetKeyPressed();
    if (key_pressed < KeyPad::NUM_OF_KEYS) {
        chip->m_regs_V[regx_indx] = key_pressed;
        MoveToNextOp(&(chip->m_pc));
    }
}

void CHIP_8::Op0x00E0(CHIP_8 *chip) {
    chip->m_pixels.reset();

    chip->m_draw_flag = true;
    
    MoveToNextOp(&(chip->m_pc));
}

void CHIP_8::Op0xDXYN(CHIP_8 *chip) {
    uint16_t opcode = chip->m_opcode;
    const uint8_t regx_indx = (opcode & REG_X) >> 8;
    const uint8_t regy_indx = (opcode & REG_Y) >> 4;
    const uint16_t pos_x = chip->m_regs_V[regx_indx];
    const uint16_t pos_y = chip->m_regs_V[regy_indx];
    const uint8_t sprite_hight = opcode & 0x000F;
    constexpr uint8_t sprite_width = 8;
    const uint16_t I = chip->m_index_reg; 
    
    chip->m_regs_V[0xF] = 0;

    for (size_t row = 0; row < sprite_hight; ++row) {
        uint8_t sprite_row = chip->m_memory[I + row];

        for (size_t col = 0; col < sprite_width; ++col) {
            constexpr uint8_t bit_col_mask = 0x80;

            if (0 != (sprite_row & (bit_col_mask >> col))) {
                size_t pixel_index = pos_x + col +
                                     ((pos_y + row) * SCREEN_WIDTH);
                if (pixel_index < NUM_OF_PIXELS) { 
                    if (chip->m_pixels[pixel_index]) {
                        chip->m_regs_V[0xF] = 1;
                    }
                    chip->m_pixels.flip(pixel_index);
                }
            }
        }
    }

    chip->m_draw_flag = true;
    
    MoveToNextOp(&(chip->m_pc));
}

const std::unordered_map<uint16_t, std::function<void(CHIP_8*)>>  CHIP_8::s_opcode_tabale =
{
    // Flow operations
    {0x1000, &Op0x1NNN},
    {0x2000, &Op0x2NNN},
    {0x00EE, &Op0x00EE},
    {0xB000, &Op0xBNNN},

    // Cond operations 
    {0x3000, &Op0x3XNN},
    {0x4000, &Op0x4XNN},
    {0x5000, &Op0x5XY0},
    {0x9000, &Op0x9XY0},

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

    // Rand
    {0xC000, &Op0xCXNN},

    // Timer
    {0xF015, &Op0xFX15},
    {0xF007, &Op0xFX07},

    // Sound
    {0xF018, &Op0xFX18},
    
    // MEM operations
    {0xA000, &Op0xANNN},
    {0xF01E, &Op0xFX1E},
    {0xF029, &Op0xFX29},
    {0xF055, &Op0xFX55},
    {0xF065, &Op0xFX65},

    // KeyOp
    {0xE09E, &Op0xEX9E},
    {0xE0A1, &Op0xEXA1},
    {0xF00A, &Op0xFX0A},

    // Display operations
    {0x00E0, &Op0x00E0},
    {0xD000, &Op0xDXYN},
    
    // BCD
    {0xF033, Op0xFX33}
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
