#ifndef CHIP_8_HPP
#define CHIP_8_HPP

#include <array>
#include <bitset>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <chrono>

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
    void SetKey(KeyPad::Keys key, bool pressed);

    std::size_t GetScreenWidth() const { return SCREEN_WIDTH; }
    std::size_t GetScreenHeight() const { return SCREEN_HEIGHT; }
    bool GetScreenPixel(std::size_t pos) const { return m_pixels.test(pos); }
    bool IsDraw() const { return m_draw_flag; }
    bool IsSound() const { return m_sound_flag; }
    void ResetDraw() { m_draw_flag = false; }
    void ResetSound() { m_sound_flag = false; }

private:
    static constexpr std::size_t MEMORY_SIZE = 4096;
    static constexpr std::size_t NUM_OF_REGISTERS = 16;
    static constexpr std::size_t SCREEN_WIDTH = 64;
    static constexpr std::size_t SCREEN_HEIGHT = 32;
    static constexpr std::size_t NUM_OF_PIXELS = SCREEN_WIDTH * SCREEN_HEIGHT;
    static constexpr std::size_t PROGRAM_START_ADDR = 0x200;
    static constexpr std::size_t NUM_OF_FONTS = 80;
    static constexpr uint16_t SIZE_OF_OPCODE = 2;
    static constexpr double TIMERS_SPEED_IN_SEC = 1.0 / 60.0;

    // Opcode masks
    static constexpr uint16_t ADDR_NNN = 0x0FFF;
    static constexpr uint16_t VAL_NN = 0x00FF;
    static constexpr uint16_t REG_X = 0x0F00;
    static constexpr uint16_t REG_Y = 0x00F0;
    
    std::array<unsigned char, MEMORY_SIZE> m_memory{0};
    std::array<unsigned char, NUM_OF_REGISTERS> m_regs_V{0};
    std::bitset<NUM_OF_PIXELS> m_pixels{0};
    Stack m_stack;
    KeyPad m_key_pad;
    uint16_t m_index_reg{0};
    uint16_t m_pc{PROGRAM_START_ADDR};
    uint16_t m_opcode{0};
    uint16_t m_delay_timer{0};
    uint16_t m_sound_timer{0};
    std::chrono::time_point<std::chrono::steady_clock> m_time;
    bool m_draw_flag{false};
    bool m_sound_flag{false};

    static void LoadFonts(std::array<unsigned char, MEMORY_SIZE>& memory);
    static void MoveToNextOp(uint16_t *pc);
    static uint16_t FindOpcodeTableIndex(uint16_t opcode);
    static uint8_t RandomNumber();
    void TickDelayTimer();
    void TickSoundTimer();
    bool IsTimerTick();
    double SecondsPassed();
    
    static void Op0x2NNN(CHIP_8 *chip);
    static void Op0x1NNN(CHIP_8 *chip);
    static void Op0x00EE(CHIP_8 *chip);
    static void Op0xBNNN(CHIP_8 *chip);

    static void Op0x3XNN(CHIP_8 *chip);
    static void Op0x4XNN(CHIP_8 *chip);
    static void Op0x5XY0(CHIP_8 *chip);
    static void Op0x9XY0(CHIP_8 *chip);

    static void Op0x6XNN(CHIP_8 *chip);
    static void Op0x7XNN(CHIP_8 *chip);

    static void Op0x8XY0(CHIP_8 *chip);
    static void Op0x8XY1(CHIP_8 *chip);
    static void Op0x8XY2(CHIP_8 *chip);
    static void Op0x8XY3(CHIP_8 *chip);
    static void Op0x8XY6(CHIP_8 *chip);
    static void Op0x8XYE(CHIP_8 *chip);

    static void Op0x8XY4(CHIP_8 *chip);
    static void Op0x8XY5(CHIP_8 *chip);
    static void Op0x8XY7(CHIP_8 *chip);

    static void Op0xANNN(CHIP_8 *chip);
    static void Op0xCXNN(CHIP_8 *chip);
    static void Op0xEX9E(CHIP_8 *chip);
    static void Op0xEXA1(CHIP_8 *chip);
    static void Op0xFX07(CHIP_8 *chip);
    static void Op0xFX0A(CHIP_8 *chip);
    static void Op0xFX15(CHIP_8 *chip);
    static void Op0xFX18(CHIP_8 *chip);
    static void Op0xFX1E(CHIP_8 *chip);
    static void Op0xFX29(CHIP_8 *chip);
    static void Op0xFX33(CHIP_8 *chip);
    static void Op0xFX55(CHIP_8 *chip);
    static void Op0xFX65(CHIP_8 *chip);

    static void Op0x00E0(CHIP_8 *chip);
    static void Op0xDXYN(CHIP_8 *chip);
    
    static const std::unordered_map<uint16_t, std::function<void(CHIP_8*)>> s_opcode_tabale;
    static const std::array<uint8_t, NUM_OF_FONTS> s_font_set;

};

} // CHIP_8

#endif // CHIP-8_HPP
