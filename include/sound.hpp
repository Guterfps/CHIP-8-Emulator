
#ifndef SOUND_HPP
#define SOUND_HPP

#include "raylib.h"

namespace Emulator
{

class SoundEffect {
public:
    SoundEffect(const char *file_path);
    ~SoundEffect();

    void Play();
    
private:
    Sound m_sound;
    
};
    
}

#endif // SOUND_HPP
