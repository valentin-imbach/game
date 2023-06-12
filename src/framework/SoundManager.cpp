
#include "SoundManager.hpp"

#define SOUND_PATH "../assets/sounds/"

std::array<Mix_Music*, SoundID::count> SoundManager::sounds = {};
int SoundManager::volume = 0;

void SoundManager::Init() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != -1) {
		LOG("Mixer initialized");
	} else {
        ERROR("Failed to initialize Mixer");
        return;
    }
    volume = Mix_VolumeMusic(-1);
    loadSounds();
}

void SoundManager::setVolume(int v) {
    Mix_VolumeMusic(v);
    volume = Mix_VolumeMusic(-1);
}

void SoundManager::playSound(SoundID::value soundId) {
    if (!soundId) return;
    Mix_PlayMusic(sounds[soundId], 1);
}

void SoundManager::loadSounds() {
    for (int i = 1; i < SoundID::count; i++) {
        SoundID::value soundId = SoundID::from_int(i);
        std::string path = SOUND_PATH + SoundID::to_string(soundId) + ".mp3";
        Mix_Music* sound = Mix_LoadMUS(path.c_str());
        if (!sound) {
            ERROR("Failed to load sound from", path);
            continue;
        }
        LOG("Sound loaded from", path);
        sounds[i] = sound;
    }
}