
#pragma once
#include <SDL2/SDL.h>
#include "utils/utils.hpp"

#define SPRITE_PATH "../assets/sprites/"

constexpr int BIT = 16;

class TextureManager {
public:
	TextureManager() = delete;
	static void Init();
	static SDL_Texture* loadTexture(std::string path);
	static void drawTexture(SDL_Texture* texture, pair source, pair size, pair position, int scale = 1, bool centered = false);
};