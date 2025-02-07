#ifndef KEY_PAD_HPP
#define KEY_PAD_HPP

#include <bitset>
#include <cstdint>

namespace Emulator
{

class KeyPad
{
public:
    enum Keys : uint8_t
    {
        KEY_0 = 0,
        KEY_1, KEY_2, KEY_3,
        KEY_4, KEY_5, KEY_6,
        KEY_7, KEY_8, KEY_9,
        KEY_A, KEY_B, KEY_C,
        KEY_D, KEY_E, KEY_F,
        NUM_OF_KEYS 
    };

    bool IsKeyPressed(Keys key) const;
    Keys GetKeyPressed() const;

private:
    std::bitset<NUM_OF_KEYS> m_keys{0};
    
};

inline bool KeyPad::IsKeyPressed(Keys key) const {
    return m_keys[key];
}

inline KeyPad::Keys KeyPad::GetKeyPressed() const {
    std::size_t i = 0;

    for (;(i < NUM_OF_KEYS) && (!m_keys.test(i)); ++i);
    
    return static_cast<Keys>(i);
}

}

#endif // KEY_PAD_HPP
