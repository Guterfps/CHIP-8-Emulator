
#ifndef MANEGER_HPP
#define MANEGER_HPP

#include "chip-8.hpp"
#include "window.hpp"
#include <cstdint>

namespace Emulator
{

class Maneger
{
public:
    Maneger();

    void Run();

private:
    static constexpr uint32_t WINDOW_WIDTH = 640;
    static constexpr uint32_t WINDOW_HEIGHT = 320;
    static constexpr const char *WINDOW_TITLE = "CHIP-8";
    static constexpr uint32_t REFRESH_RATE = 60;
    static constexpr uint32_t PIXEL_SIZE = 10;

    enum State : uint8_t {MENU = 0, EMULATE};

    CHIP_8 m_chip;
    Window m_window;
    State m_state;

    void RunMenu();
    void RunEmulation();
    void TakeInputs();
    
};

}

#endif // MANEGER_HPP
