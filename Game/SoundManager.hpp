//
//  SoundManager.hpp
//  Game
//
//  Created by Valentin Imbach on 29.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include <SDL2_mixer/SDL_mixer.h>
#include "MessagingSystem.hpp"

class SoundManager : public Observer {
public:
    static void Init();
    static SoundManager manager;
    static bool playSound(const char* path);
    static void setVolume(int v);
    static std::map<std::string, Mix_Music*> sounds;
    static int volume;
    bool onMessage(const Message& message) override;
};
