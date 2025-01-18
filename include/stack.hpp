#ifndef STACK_HPP
#define STACK_HPP

#include <array>
#include <cstdint>

namespace Emulation
{

class Stack 
{
public:


private:
    static constexpr std::size_t NUM_OF_LEVELS = 16;
    
    std::array<uint16_t, NUM_OF_LEVELS> m_levels;
    uint16_t m_sp{0};

};

}

#endif // STACK_HPP