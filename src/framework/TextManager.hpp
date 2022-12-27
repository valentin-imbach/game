
#pragma once
#include "utils.hpp"
#include <SDL2_ttf/SDL_ttf.h>

#define FONT_PATH "../assets/fonts/"

struct Text {
	std::string text;
	int style;
	SDL_Color colour;

    Text(std::string text,  int style = TTF_STYLE_NORMAL, SDL_Color colour = {255, 255, 255, 255}) : text(text), style(style), colour(colour) {}
};

class TextManager {
public:
	TextManager();
	static TTF_Font* font;
	static SDL_Color color;

	static void Init();

	static pair textSize(std::string& text);

	static void drawText(std::string& text, pair position, bool centred = false, SDL_Color colour = {255, 255, 255, 255});
	static void drawText(Text& text, pair position, bool centred = false);
	static void loadFont(std::string path, int size);
};