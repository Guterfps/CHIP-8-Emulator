
#ifndef MENU_HPP
#define MENU_HPP

#include <cstddef>
#include <array>

#include "raylib.h"

namespace Emulator
{

class Menu {
public:
    Menu(float x, float y, float width, float hieght);
    
    void Update();
    void Draw();

    const char *GetFilePath() const {
        return m_file_path.data();
    }

private:
    static constexpr size_t MAX_INPUT_PATH_SIZE = 256;

    std::array<char, MAX_INPUT_PATH_SIZE> m_file_path{'\0'};
    size_t m_letters_counter{0};
    Rectangle m_text_box;
        
};

}


#endif // MENU_HPP
