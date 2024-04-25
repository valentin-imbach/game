
#pragma once
#include "utils.hpp"
#include <SDL2_ttf/SDL_ttf.h>

#define FONT_PATH "../assets/fonts/"

struct Text {
	Text(std::string text, int style = TTF_STYLE_NORMAL, Colour colour = {255, 255, 255, 255});
	std::string text;
	int style;
	Colour colour;	
};

class TextManager {
public:
	TextManager() = delete;
	static TTF_Font* font;
	static Colour colour;

	static void Init();
	static void cleanup();
	static pair textSize(std::string& text);

	static void drawText(std::string& text, pair position, bool centred = false, Colour colour = {255, 255, 255, 255});
	static void drawText(Text& text, pair position, bool centred = false);
	static void loadFont(std::string path, int size);
};