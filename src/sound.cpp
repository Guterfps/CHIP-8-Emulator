
#include "sound.hpp"
#include <raylib.h>

namespace Emulator
{

SoundEffect::SoundEffect(const char *file_path)
{
    InitAudioDevice();

    m_sound = LoadSound(file_path);
}

SoundEffect::~SoundEffect() {
    UnloadSound(m_sound);
    CloseAudioDevice();
}

void SoundEffect::Play() {
    PlaySound(m_sound);
}

} // Emulator
