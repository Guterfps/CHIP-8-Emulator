#ifndef KEY_PAD_HPP
#define KEY_PAD_HPP

#include <array>
#include <cstdint>

namespace Emulator
{

class KeyPad
{
public:


private:
    static constexpr std::size_t NUM_OF_KEYS = 16;

    std::array<uint16_t, NUM_OF_KEYS> m_keys;
};

}

#endif // KEY_PAD_HPP