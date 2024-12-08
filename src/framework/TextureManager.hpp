
#pragma once
#include <SDL3/SDL.h>
#include "utils.hpp"

#define SPRITE_PATH "../assets/sprites/"

constexpr int BIT = 16;

struct TextureStyle {
	bool centered = true;
	float alpha = 1;
	SDL_FlipMode flip = SDL_FLIP_NONE;
	Colour tint;
	bool outline = false;
	float angle = 0;
	pair pivot = {0, 0};
};

class TextureManager {
public:
	TextureManager() = delete;
	static SDL_Texture* loadTexture(std::string path, bool outline = false);
	static void drawTexture(SDL_Texture* src, SDL_Texture* dst, vec spos, vec ssize, vec dpos, vec dsize, TextureStyle style = TextureStyle());
	static bool ison(vec point, SDL_Texture* src, SDL_Texture* dst, vec spos, vec ssize, vec dpos, vec dsize, TextureStyle style = TextureStyle());
	static void drawRect(vec position, vec size, Colour colour = {255, 0, 0, 255}, bool centered = true, bool filled = false);
	static void drawCirc(vec position, int radius, Colour colour = {255, 0, 0, 255});

	// static void saveTexture(SDL_Texture* texture, std::filesystem::path path);

	static SDL_Texture* createTexture(pair size, Colour colour = {0, 0, 0, 0});
	static void outlineSurface(SDL_Surface* surface);
	static SDL_Texture* lightTexture;
};