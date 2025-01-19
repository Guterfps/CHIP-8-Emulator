#ifndef CHIP_8_HPP
#define CHIP_8_HPP

#include <array>
#include <bitset>
#include <cstdint>
#include <functional>
#include <unordered_map>

#include "stack.hpp"
#include "key_pad.hpp"

namespace Emulator
{

class CHIP_8
{
public:
    enum class Status : int 
    {
        ERROR = -1,
        SUCESS = 0
    };

    CHIP_8();

    Status LoadBin(const char *file_path);

    void EmulateCycle();

private:
    static constexpr std::size_t MEMORY_SIZE = 4096;
    static constexpr std::size_t NUM_OF_REGIDTERS = 16;
    static constexpr std::size_t SCREEN_WIDTH = 32;
    static constexpr std::size_t SCREEN_HIGHT = 64;
    static constexpr std::size_t NUM_OF_PIXELS = SCREEN_HIGHT * SCREEN_WIDTH;
    static constexpr std::size_t PROGRAM_START_ADDR = 0x200;
    static constexpr std::size_t NUM_OF_FONTS = 80;
    
    std::array<unsigned char, MEMORY_SIZE> m_memory{0};
    std::array<unsigned char, NUM_OF_REGIDTERS> m_regs_V{0};
    std::bitset<NUM_OF_PIXELS> m_pixels{0};
    std::unordered_map<uint16_t, std::function<void(CHIP_8*)>> m_opcodes_table;
    Stack m_stack;
    KeyPad m_key_pad;
    uint16_t m_index_reg{0};
    uint16_t m_pc{PROGRAM_START_ADDR};
    uint16_t m_opcode{0};
    uint16_t m_delay_timer{0};
    uint16_t m_sound_timer{0};
    bool m_draw_flag{false};

    static void LoadFonts(std::array<unsigned char, MEMORY_SIZE>& memory);
    static std::unordered_map<uint16_t, std::function<void(CHIP_8*)>> InitOpcodesTable();

    static void Op0x200(CHIP_8 *chip);

    static constexpr std::array<uint8_t, NUM_OF_FONTS> s_font_set = 
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

};

} // CHIP_8

#endif // CHIP-8_HPP