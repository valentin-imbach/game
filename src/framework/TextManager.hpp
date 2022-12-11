
#pragma once
#include "utils.hpp"
#include <SDL2_ttf/SDL_ttf.h>

#define FONT_PATH "../assets/fonts/"

class TextManager {
public:
	TextManager();
	static TTF_Font* font;
	static SDL_Color color;

	static void Init();

	static pair textSize(std::string& text);

	static void drawText(std::string& text, pair position, bool centred = false, SDL_Color colour = {255, 255, 255, 255});
	static void loadFont(std::string path, int size);
};