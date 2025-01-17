
#include "SoundManager.hpp"
#include "Window.hpp"

std::filesystem::path SoundManager::soundPath = "assets/sounds";
std::filesystem::path SoundManager::trackPath = "assets/tracks";

std::array<Mix_Chunk*, SoundId::count> SoundManager::sounds = {};
std::array<Mix_Chunk*, TrackId::count> SoundManager::tracks = {};
int SoundManager::volume = 0;

void SoundManager::Init() {
	SDL_AudioSpec spec = {MIX_DEFAULT_FORMAT, 2, 22050};
	if (!Mix_OpenAudio(0, &spec)) {
		ERROR("Failed to initialize Mixer");
		return;
	}
	LOG("Mixer initialized");
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
	LOG("Mixer cleaned up");
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
		auto path = Window::instance->root / SoundManager::soundPath / SoundId::to_string(soundId) += ".wav";
		Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
		if (!sound) {
			ERROR("Failed to load sound from", path);
			continue;
		}
		// LOG("Sound loaded from", path);
		sounds[i] = sound;
	}
}

void SoundManager::loadTracks() {
	for (uint i = 1; i < TrackId::count; i++) {
		TrackId::value trackId = TrackId::from_int(i);
		auto path = Window::instance->root / SoundManager::trackPath / TrackId::to_string(trackId) += ".wav";
		Mix_Chunk* track = Mix_LoadWAV(path.c_str());
		if (!track) {
			ERROR("Failed to load track from", path);
			continue;
		}
		// LOG("Track loaded from", path);
		tracks[i] = track;
	}
}