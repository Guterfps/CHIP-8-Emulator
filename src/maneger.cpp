
#include <cstdint>
#include <raylib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include "maneger.hpp"
#include "chip-8.hpp"
#include "key_pad.hpp"

namespace Emulator
{

Maneger::Maneger()
: m_chip()
, m_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, REFRESH_RATE)
, m_menu(WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 4.0f,
         WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 10.0f)
, m_sound(SOUND_FILE_PATH)
, m_state(MENU)
{
}

void Maneger::Run() {

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(reinterpret_cast<void (*)(void *)>(&Maneger::LoopWraper),
                                 this, 0 , 1);
#else
    while (!WindowShouldClose()) {
        LoopFunc();
    }
#endif

}

void Maneger::LoopWraper(Maneger *self) {
    self->LoopFunc();
}

void Maneger::LoopFunc() {
    switch (m_state)
    {
        case MENU:
            RunMenu();
            break;    
        case EMULATE:
            RunEmulation();
            break;
        default:
            break;
    }
}

void Maneger::RunMenu() {
    m_menu.Update();

    if (IsKeyPressed(KEY_ENTER)) {
        m_chip = CHIP_8();

        if (CHIP_8::Status::SUCESS ==
            m_chip.LoadBin(m_menu.GetFilePath())) {
            m_state = EMULATE;
        }
    }
    
    m_menu.Draw();
}

void Maneger::RunEmulation() {
    while (!m_chip.IsDraw() && (m_state == EMULATE)) {
        TakeInputs();
        PollInputEvents();
        m_chip.EmulateCycle();
    }
    
    DrawScreen();
    m_chip.ResetDraw();

    if (m_chip.IsSound()) {
        m_sound.Play();
        m_chip.ResetSound();
    }
}

void Maneger::DrawScreen() {
    const uint32_t pixel_size = WINDOW_HEIGHT / m_chip.GetScreenHeight();

    BeginDrawing();

    for (size_t row = 0; row < m_chip.GetScreenHeight(); ++row) {
        for (size_t col = 0; col < m_chip.GetScreenWidth(); ++col) {
            Color pixel_color =
                m_chip.GetScreenPixel(row * m_chip.GetScreenWidth() + col) ?
                WHITE : BLACK;
            DrawRectangle(col * pixel_size,
                          row * pixel_size,
                          pixel_size, pixel_size,
                          pixel_color);
        }
    }

    if (m_show_fps) {
        DrawFPS(10, 10);
    }

    EndDrawing();
}

void Maneger::TakeInputs() {
    PressedKeys();
    ReleasedKeys();

    if (IsKeyPressed(KEY_TAB)) {
        m_state = MENU;
    }

    m_show_fps ^= IsKeyPressed(KEY_LEFT_SHIFT);
    
}

void Maneger::PressedKeys() {
    if (IsKeyPressed(KEY_ONE)) {
        m_chip.SetKey(KeyPad::KEY_1, true);
    }
    if (IsKeyPressed(KEY_TWO)) {
        m_chip.SetKey(KeyPad::KEY_2, true);
    }
    if (IsKeyPressed(KEY_THREE)) {
        m_chip.SetKey(KeyPad::KEY_3, true);
    }
    if (IsKeyPressed(KEY_FOUR)) {
        m_chip.SetKey(KeyPad::KEY_C, true);
    }
    if (IsKeyPressed(KEY_Q)) {
        m_chip.SetKey(KeyPad::KEY_4, true);
    }
    if (IsKeyPressed(KEY_W)) {
        m_chip.SetKey(KeyPad::KEY_5, true);
    }
    if (IsKeyPressed(KEY_E)) {
        m_chip.SetKey(KeyPad::KEY_6, true);
    }
    if (IsKeyPressed(KEY_R)) {
        m_chip.SetKey(KeyPad::KEY_D, true);
    }
    if (IsKeyPressed(KEY_A)) {
        m_chip.SetKey(KeyPad::KEY_7, true);
    }
    if (IsKeyPressed(KEY_S)) {
        m_chip.SetKey(KeyPad::KEY_8, true);
    }
    if (IsKeyPressed(KEY_D)) {
        m_chip.SetKey(KeyPad::KEY_9, true);
    }
    if (IsKeyPressed(KEY_F)) {
        m_chip.SetKey(KeyPad::KEY_E, true);
    }
    if (IsKeyPressed(KEY_Z)) {
        m_chip.SetKey(KeyPad::KEY_A, true);
    }
    if (IsKeyPressed(KEY_X)) {
        m_chip.SetKey(KeyPad::KEY_0, true);
    }
    if (IsKeyPressed(KEY_C)) {
        m_chip.SetKey(KeyPad::KEY_B, true);
    }
    if (IsKeyPressed(KEY_V)) {
        m_chip.SetKey(KeyPad::KEY_F, true);
    }
}

void Maneger::ReleasedKeys() {
    if (IsKeyReleased(KEY_ONE)) {
        m_chip.SetKey(KeyPad::KEY_1, false);
    }
    if (IsKeyReleased(KEY_TWO)) {
        m_chip.SetKey(KeyPad::KEY_2, false);
    }
    if (IsKeyReleased(KEY_THREE)) {
        m_chip.SetKey(KeyPad::KEY_3, false);
    }
    if (IsKeyReleased(KEY_FOUR)) {
        m_chip.SetKey(KeyPad::KEY_C, false);
    }
    if (IsKeyReleased(KEY_Q)) {
        m_chip.SetKey(KeyPad::KEY_4, false);
    }
    if (IsKeyReleased(KEY_W)) {
        m_chip.SetKey(KeyPad::KEY_5, false);
    }
    if (IsKeyReleased(KEY_E)) {
        m_chip.SetKey(KeyPad::KEY_6, false);
    }
    if (IsKeyReleased(KEY_R)) {
        m_chip.SetKey(KeyPad::KEY_D, false);
    }
    if (IsKeyReleased(KEY_A)) {
        m_chip.SetKey(KeyPad::KEY_7, false);
    }
    if (IsKeyReleased(KEY_S)) {
        m_chip.SetKey(KeyPad::KEY_8, false);
    }
    if (IsKeyReleased(KEY_D)) {
        m_chip.SetKey(KeyPad::KEY_9, false);
    }
    if (IsKeyReleased(KEY_F)) {
        m_chip.SetKey(KeyPad::KEY_E, false);
    }
    if (IsKeyReleased(KEY_Z)) {
        m_chip.SetKey(KeyPad::KEY_A, false);
    }
    if (IsKeyReleased(KEY_X)) {
        m_chip.SetKey(KeyPad::KEY_0, false);
    }
    if (IsKeyReleased(KEY_C)) {
        m_chip.SetKey(KeyPad::KEY_B, false);
    }
    if (IsKeyReleased(KEY_V)) {
        m_chip.SetKey(KeyPad::KEY_F, false);
    }
}

} // Emulator
