
#include "SoundManager.hpp"

#define SOUND_PATH "../assets/sounds/"
#define TRACK_PATH "../assets/tracks/"

std::array<Mix_Chunk*, SoundId::count> SoundManager::sounds = {};
std::array<Mix_Chunk*, TrackId::count> SoundManager::tracks = {};
int SoundManager::volume = 0;

void SoundManager::Init() {
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != -1) {
		LOG("Mixer initialized");
	} else {
		ERROR("Failed to initialize Mixer");
		return;
	}
	volume = Mix_VolumeMusic(-1);
	Mix_AllocateChannels(2 * TrackId::count);

    for (uint i = 1; i < 2 * TrackId::count; i++) Mix_Volume(i, MIX_MAX_VOLUME / 10);

	loadSounds();
	loadTracks();
}

void SoundManager::cleanup() {
	for (uint i = 1; i < SoundId::count; i++) {
		if (sounds[i]) Mix_FreeChunk(sounds[i]);
	}

	for (uint i = 1; i < TrackId::count; i++) {
		if (tracks[i]) Mix_FreeChunk(tracks[i]);
	}
	
	Mix_Quit();
}

void SoundManager::setVolume(int v) {
	Mix_VolumeMusic(v);
	volume = Mix_VolumeMusic(-1);
}

void SoundManager::playSound(SoundId::value soundId) {
	if (!soundId) return;
	Mix_PlayChannel(0, sounds[soundId], 0);
}

void SoundManager::playTrack(TrackId::value trackId, int channel, int count) {
	if (!trackId) return;
	Mix_PlayChannel(channel, tracks[trackId], count - 1);
}

void SoundManager::loadSounds() {
	for (uint i = 1; i < SoundId::count; i++) {
		SoundId::value soundId = SoundId::from_int(i);
		std::string path = SOUND_PATH + SoundId::to_string(soundId) + ".wav";
		Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
		if (!sound) {
			ERROR("Failed to load sound from", path);
			continue;
		}
		LOG("Sound loaded from", path);
		sounds[i] = sound;
	}
}

void SoundManager::loadTracks() {
	for (uint i = 1; i < TrackId::count; i++) {
		TrackId::value trackId = TrackId::from_int(i);
		std::string path = TRACK_PATH + TrackId::to_string(trackId) + ".wav";
		Mix_Chunk* track = Mix_LoadWAV(path.c_str());
		if (!track) {
			ERROR("Failed to load track from", path);
			continue;
		}
		LOG("Track loaded from", path);
		tracks[i] = track;
	}
}