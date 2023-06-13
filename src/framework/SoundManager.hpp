
#pragma once
#include <SDL2_mixer/SDL_mixer.h>
#include "SDL2/SDL_timer.h"
#include "utils.hpp"

ENUM(SoundId,
TREE,
ROCK,
BUSH,
PICKUP)

ENUM(TrackId,
BASE,
CELLO,
VIOLA,
VIOLIN_1,
VIOLIN_2,
VIOLIN_3,
OBOE,
CLARINET,
PICOLO,
FLUTE,
BASOON,
HARP,
MARIMBA,
CELESTE)

class SoundManager {
public:
    static void Init();
    static void loadSounds();
    static void loadTracks();
    static void playSound(SoundId::value soundId);
    static void playTrack(TrackId::value trackId, int channel, int count = 1);
    static void setVolume(int v);
    static std::array<Mix_Chunk*, SoundId::count> sounds;
    static std::array<Mix_Chunk*, TrackId::count> tracks;
    static int volume;
};

struct TrackMix {
    TrackMix(int loops) : loops(loops) {
        change();
    }

    std::bitset<TrackId::count> activeTracks;
    int loops;
    uint32_t lastChange;
    uint32_t duration = 8000;
    float variance = 0.2f;
    bool parity = false;

    void change() {
        for (int i = 1; i < TrackId::count; i++) {
            if (bernoulli(rand(), variance)) activeTracks[i] = !activeTracks[i];
            if (activeTracks[i]) {
                TrackId::value trackId = TrackId::from_int(i);
                SoundManager::playTrack(trackId, i + parity * TrackId::count, loops);
                LOG(TrackId::to_string(trackId));
            }
        }
        lastChange = SDL_GetTicks();
        parity = !parity;
    }

    void update() {
        uint32_t ticks = SDL_GetTicks();
        if (ticks > lastChange + loops * duration) change();
    }
};