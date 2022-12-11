
#pragma once
#include <SDL2/SDL.h>
#include "utils.hpp"

#define SPRITE_PATH "../assets/sprites/"

constexpr int BIT = 16;

class TextureManager {
public:
	TextureManager() = delete;
	static void Init();
	static SDL_Texture* loadTexture(std::string path);
	static void drawTexture(SDL_Texture* texture, pair source, pair size, pair position, int scale = 1, bool centered = false);
	static void drawRect(pair position, pair size, SDL_Colour colour = {255, 0, 0, 255}, bool centered = true, bool filled = false);
};