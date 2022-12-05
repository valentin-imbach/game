
#pragma once
#include "utils/utils.hpp"
#include <SDL2_ttf/SDL_ttf.h>

class TextManager {
public:
	TextManager();
	static TTF_Font* font;
	static SDL_Color color;

	static void Init();

	static pair textSize(std::string& text);

	static void drawText(std::string& text, pair position, bool centred = false, SDL_Color colour = {255, 255, 255, 255});
	static void loadFont(const char* path, int size);
};