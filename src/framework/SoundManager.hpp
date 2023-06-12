
#pragma once
#include <SDL2_mixer/SDL_mixer.h>
#include "utils.hpp"

ENUM(SoundID,
TREE,
ROCK,
BUSH,
PICKUP)

class SoundManager {
public:
    static void Init();
    static void loadSounds();
    static void playSound(SoundID::value soundId);
    static void setVolume(int v);
    static std::array<Mix_Music*, SoundID::count> sounds;
    static int volume;
};
