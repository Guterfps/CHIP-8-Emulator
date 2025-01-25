#ifndef STACK_HPP
#define STACK_HPP

#include <array>
#include <cstdint>
#include <sys/types.h>

#ifndef NDEBUG
    #include <iostream>
#endif        

namespace Emulator
{

class Stack 
{
public:

    void Push(uint16_t data) {
    #ifndef NDEBUG        
        if (m_sp >= NUM_OF_STACK_BLOCKS) {
            std::cout << "stack overflow\n";
        }
    #endif
    
        m_blocks[m_sp++] = data;
    }

    uint16_t Pop() {
    #ifndef NDEBUG
        if (0 == m_sp) {
            std::cout << "stack is empty\n";
        }
    #endif

        uint16_t ret_val = m_blocks[--m_sp];

        return ret_val;
    }

    void SetSP(uint16_t move_to) {
        m_sp = move_to;
    }

    void MoveSP(int16_t move_by) {
        m_sp += move_by;
    }

    uint16_t GetSP() const {
        return m_sp;
    }

private:
    static constexpr std::size_t NUM_OF_STACK_BLOCKS = 16;
    
    std::array<uint16_t, NUM_OF_STACK_BLOCKS> m_blocks{0};
    uint16_t m_sp{0};

};

}

#endif // STACK_HPP
