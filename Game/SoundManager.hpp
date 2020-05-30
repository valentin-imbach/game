//
//  SoundManager.hpp
//  Game
//
//  Created by Valentin Imbach on 29.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include <SDL2_mixer/SDL_mixer.h>

class SoundManager {
public:
    static void Init();
    static void play();
    static void setVolume(int v);
    static Mix_Music* music;
    static int volume;
};
