
#pragma once
#include "utils.hpp"

#include <SDL3_ttf/SDL_ttf.h>

struct Text {
	Text(std::string text = "", int style = 0, Colour colour = {255, 255, 255, 255});
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
	static pair getTextSize(const std::string& text);

	static void drawText(std::string& text, vec position, bool centred = false, Colour colour = {255, 255, 255, 255});
	static void drawText(Text& text, vec position, bool centred = false);
	static void loadFont(std::string name, int size);

	static std::filesystem::path fontPath;
};