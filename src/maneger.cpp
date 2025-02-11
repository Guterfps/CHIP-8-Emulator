
#include <cstdint>
#include <raylib.h>

#include "maneger.hpp"
#include "chip-8.hpp"
#include "key_pad.hpp"

namespace Emulator
{

Maneger::Maneger()
: m_chip()
, m_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, REFRESH_RATE)
, m_state(MENU)
{
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
    constexpr size_t MAX_FILE_PATH = 256;
    std::array<char, MAX_FILE_PATH> file_path = {0};
    bool is_valid = true;
    
    if (IsFileDropped()) {
        const FilePathList dropped_file = LoadDroppedFiles();
        TextCopy(file_path.data(), dropped_file.paths[0]);
        UnloadDroppedFiles(dropped_file);

        std::cout << "file path: " << file_path.data() << '\n';

        if (CHIP_8::Status::SUCESS ==
            m_chip.LoadBin(file_path.data())) {
            m_state = EMULATE;
        }
        else {
            is_valid = false;
        }
    }
    
    BeginDrawing();

    ClearBackground(BLACK);
    
    DrawText("Drop a Game File here",
             WINDOW_WIDTH / 4.0,
             WINDOW_HEIGHT / 2.0,
             WINDOW_HEIGHT / 10.0,
             WHITE);

    if (!is_valid) {
        DrawText("Invalid File",
                 WINDOW_WIDTH / 4.0,
                 WINDOW_HEIGHT / 1.5,
                 WINDOW_HEIGHT / 10.0,
                 WHITE);
    }

    EndDrawing();
}

void Maneger::RunEmulation() {
    m_chip.EmulateCycle();

    if (m_chip.IsDraw()) {
        DrawScreen();
        m_chip.ResetDraw();
    }
    else {
        PollInputEvents();
    }

    if (m_chip.IsSound()) {

        m_chip.ResetSound();
    }

    TakeInputs();
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

    EndDrawing();
}

void Maneger::TakeInputs() {
    PressedKeys();
    ReleasedKeys();
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
