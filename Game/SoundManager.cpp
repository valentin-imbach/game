//
//  SoundManager.cpp
//  Game
//
//  Created by Valentin Imbach on 29.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "tools.h"
#include "SoundManager.hpp"

Mix_Music* SoundManager::music = NULL;
int SoundManager::volume = 0;

void SoundManager::Init() {
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) != -1 ) {
        LOG("Mixer initialized");
    } else {
        ERROR("Failed to initialize Mixer");
    }

    volume = Mix_VolumeMusic(-1);
    
    const char* path = "music.mp3";
    music = Mix_LoadMUS(path);
    if (music != NULL) {
        LOG("Audio loaded from", path);
    } else {
        ERROR("Failed to load Audio from",path);
    }
}

void SoundManager::setVolume(int v) {
    Mix_VolumeMusic(v);
    volume = Mix_VolumeMusic(-1);
}

void SoundManager::play() {
    Mix_PlayMusic( music, -1 );
}
