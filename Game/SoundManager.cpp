//
//  SoundManager.cpp
//  Game
//
//  Created by Valentin Imbach on 29.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "tools.h"
#include "SoundManager.hpp"

auto SoundManager::sounds = std::map<std::string, Mix_Music*>();
SoundManager SoundManager::manager = SoundManager();
int SoundManager::volume = 0;

void SoundManager::Init() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != -1 ) LOG("Mixer initialized");
    else {
        ERROR("Failed to initialize Mixer");
        return;
    }
    volume = Mix_VolumeMusic(-1);
    manager.subscribe(MessageType::SOUND);
}

void SoundManager::setVolume(int v) {
    Mix_VolumeMusic(v);
    volume = Mix_VolumeMusic(-1);
}

bool SoundManager::playSound(const char* path) {
    if (!path) return false;
    if (sounds.find(path) != sounds.end()) {
        Mix_PlayMusic(sounds[path], 1);
        return true;
    }
    Mix_Music* sound = Mix_LoadMUS(path);
    if (!sound) {
        ERROR("Failed to load sound from ", path);
        return false;
    }
    sounds[path] = Mix_LoadMUS(path);
    LOG("Sound loaded from", path);
    Mix_PlayMusic(sounds[path], 1);
    return true;
}

bool SoundManager::onMessage(const Message& message) {
    if (message.type == MessageType::SOUND) {
        const SoundMessage &msg = static_cast<const SoundMessage&>(message);
        return playSound(msg.path);
    }
    return false;
}
