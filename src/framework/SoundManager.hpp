
#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_timer.h>
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
    static void cleanup();
    static void loadSounds();
    static void loadTracks();
    static void playSound(SoundId::value soundId);
    static void playTrack(TrackId::value trackId, int channel, int count = 1);
    static void setVolume(int v);
    static std::array<Mix_Chunk*, SoundId::count> sounds;
    static std::array<Mix_Chunk*, TrackId::count> tracks;
    static int volume;
    static std::filesystem::path soundPath;
    static std::filesystem::path trackPath;
};

struct TrackMix {
    TrackMix(int loops) : loops(loops) {
        change();
    }

    std::bitset<TrackId::count> activeTracks;
    int loops;
    uint lastChange;
    uint duration = 8000;
    float variance = 0.2f;
    bool parity = false;

    void change() {
        uint ticks = SDL_GetTicks();
        for (uint i = 1; i < TrackId::count; i++) {
            if (noise::bernoulli(ticks, variance)) activeTracks[i] = !activeTracks[i];
            if (activeTracks[i]) {
                TrackId::value trackId = TrackId::from_int(i);
                SoundManager::playTrack(trackId, i + parity * TrackId::count, loops);
                // LOG(TrackId::to_string(trackId));
            }
        }
        lastChange = ticks;
        parity = !parity;
    }

    void update() {
        uint ticks = SDL_GetTicks();
        if (ticks > lastChange + loops * duration) change();
    }
};