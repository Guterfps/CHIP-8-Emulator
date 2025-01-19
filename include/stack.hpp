#ifndef STACK_HPP
#define STACK_HPP

#include <array>
#include <cstdint>

namespace Emulator
{

class Stack 
{
public:

    void SetSP(uint16_t move_to) {
        m_sp = move_to;
    }

    void MoveSP(int16_t move_by) {
        m_sp += move_by;
    }

    uint16_t GetSP() const {
        return m_sp;
    }

    uint16_t& operator[](std::size_t stack_frame) {
        return m_frames[stack_frame];
    }

private:
    static constexpr std::size_t NUM_OF_STACK_FRAMES = 16;
    
    std::array<uint16_t, NUM_OF_STACK_FRAMES> m_frames{0};
    uint16_t m_sp{0};

};

}

#endif // STACK_HPP