
#include "maneger.hpp"
#include "key_pad.hpp"
#include <raylib.h>

namespace Emulator
{

Maneger::Maneger()
: m_chip()
, m_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, REFRESH_RATE)
, m_state(EMULATE)
{
    m_chip.LoadBin("./games/PONG2"); // TODO: load in menu
}

void Maneger::Run() {
    while (!WindowShouldClose()) {
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
}

void Maneger::RunMenu() {
    constexpr float text_box_width = 30;
    constexpr float text_box_height= 10;
    
    static Rectangle text_box = {
        WINDOW_WIDTH / 2.0f,
        WINDOW_HEIGHT / 2.0f,
        text_box_width,
        text_box_height    
    };
    
}

void Maneger::RunEmulation() {
    m_chip.EmulateCycle();

    if (m_chip.IsDraw()) {
        BeginDrawing();

        for (size_t row = 0; row < m_chip.GetScreenHeight(); ++row) {
            for (size_t col = 0; col < m_chip.GetScreenWidth(); ++col) {
                Color pixel_color =
                    m_chip.GetScreenPixel(row * m_chip.GetScreenWidth() + col) ?
                    WHITE : BLACK;
                DrawRectangle(col * PIXEL_SIZE,
                              row * PIXEL_SIZE,
                              PIXEL_SIZE, PIXEL_SIZE,
                              pixel_color);
            }
        }

        EndDrawing();
        m_chip.ResetDraw();
    }

    if (m_chip.IsSound()) {

        m_chip.ResetSound();
    }

    TakeInputs();
}

void Maneger::TakeInputs() {
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
