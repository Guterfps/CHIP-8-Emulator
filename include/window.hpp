
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cstdint>

#include "raylib.h"

namespace Emulator
{

class Window
{
public:
    Window(uint32_t width, uint32_t height, const char *title, uint32_t fps);
    ~Window();
    
private:
    
};

inline Window::Window(uint32_t width, uint32_t height, const char *title, uint32_t fps) {
    InitWindow(width, height, title);
    SetTargetFPS(fps);
}

inline Window::~Window() {
    CloseWindow();
}

}

#endif // WINDOW_HPP
