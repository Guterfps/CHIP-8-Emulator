#ifndef CHIP_8_HPP
#define CHIP_8_HPP

#include <array>

namespace CHIP_8
{

class CHIP_8
{
public:


private:
    static constexpr std::size_t MEMORY = 4096;
    static constexpr std::size_t NUM_OF_REGIDTERS = 16;

    std::array<unsigned char, MEMORY> m_memory;
    std::array<unsigned char, NUM_OF_REGIDTERS> m_registers_V; 
};

} // CHIP_8

#endif // CHIP-8_HPP