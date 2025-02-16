
#include "menu.hpp"
#include <raylib.h>

namespace Emulator
{

Menu::Menu(float x, float y, float width, float hieght)
: m_text_box{x, y, width, hieght}
{}

void Menu::Update() {
    constexpr int MIN_KEY_VAL = 32;
    constexpr int MAX_KEY_VAL = 125;
    
    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= MIN_KEY_VAL) &&
            (key <= MAX_KEY_VAL) &&
            (m_letters_counter < MAX_INPUT_PATH_SIZE)) {
            m_file_path[m_letters_counter++] = static_cast<char>(key);
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) &&
        (m_letters_counter > 0)) {
        m_file_path[--m_letters_counter] = '\0';
    }
}

void Menu::Draw() {
    constexpr int CHAR_SIZE = 5;
    constexpr int FONT_SIZE = CHAR_SIZE * CHAR_SIZE;

    BeginDrawing();

    ClearBackground(BLACK);

    DrawText("Enter Game File path:",
             m_text_box.x,
             m_text_box.y - CHAR_SIZE * 5,
             FONT_SIZE, WHITE);

    DrawRectangleLinesEx(m_text_box, 1.0f, RAYWHITE);

    DrawText(m_file_path.data(),
             m_text_box.x + CHAR_SIZE,
             m_text_box.y + CHAR_SIZE,
             FONT_SIZE, WHITE);

    EndDrawing();
}

} // emulation
