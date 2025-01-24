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
    Stack m_stack;
    KeyPad m_key_pad;
    uint16_t m_index_reg{0};
    uint16_t m_pc{PROGRAM_START_ADDR};
    uint16_t m_opcode{0};
    uint16_t m_delay_timer{0};
    uint16_t m_sound_timer{0};
    bool m_draw_flag{false};

    static void LoadFonts(std::array<unsigned char, MEMORY_SIZE>& memory);

    static void Op0x200(CHIP_8 *chip);

    static const std::unordered_map<uint16_t, std::function<void(CHIP_8*)>> s_opcode_tabale;
    static const std::array<uint8_t, NUM_OF_FONTS> s_fonts_set;

};

} // CHIP_8

#endif // CHIP-8_HPP
