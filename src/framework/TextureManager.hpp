
#pragma once
#include <SDL2/SDL.h>
#include "utils.hpp"

#define SPRITE_PATH "../assets/sprites/"

constexpr int BIT = 16;

struct TextureStyle {
	bool centered = true;
	float alpha = 1;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_Color tint = {255, 255, 255};
	bool outline = false;
};

class TextureManager {
public:
	TextureManager() = delete;
	static SDL_Texture* loadTexture(std::string path, bool outline = false);
	static void drawTexture(SDL_Texture* src, SDL_Texture* dst, pair spos, pair ssize, pair dpos, pair dsize, TextureStyle style = TextureStyle());
	static void drawRect(pair position, pair size, SDL_Colour colour = {255, 0, 0, 255}, bool centered = true, bool filled = false);
	static void drawCirc(pair position, int radius, SDL_Colour colour = {255, 0, 0, 255});

	static SDL_Texture* createTexture(pair size, SDL_Colour colour = {0, 0, 0, 0});
	static void outlineSurface(SDL_Surface* surface);
	static SDL_Texture* lightTexture;
};