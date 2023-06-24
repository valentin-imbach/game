
#include "TextManager.hpp"
#include "Window.hpp"
#include <sstream>

TTF_Font* TextManager::font = nullptr;

Text::Text(std::string text, int style, SDL_Color colour)
	: text(text), style(style), colour(colour) {}

void TextManager::Init() {
	if (TTF_Init() != 0) {
		ERROR("Failed to initialize TTF");
		return;
	}
	LOG("TextManager initialized");
	TextManager::loadFont("font.ttf", 32);
}

void TextManager::cleanup() {
	if (font) TTF_CloseFont(font);
	TTF_Quit();
}

void TextManager::loadFont(std::string path, int size) {
	font = TTF_OpenFont((FONT_PATH + path).c_str(), size);
	if (!font) {
		ERROR("Failed to load font from", path)
		return;
	}
	LOG("Font loaded from", path);
}

pair TextManager::textSize(std::string& text) {
	pair size(0, 0);
	if (TTF_SizeText(font, text.c_str(), &size.x, &size.y)) ERROR("Failed to determine size of string", text);
	return size;
}

void TextManager::drawText(std::string& text, pair position, bool centred, SDL_Color colour) {
	if (!font || text.empty()) return;
	std::vector<std::string> lines;
	std::istringstream iss(text);
	std::string line;
	while (std::getline(iss, line)) lines.push_back(line);

	pair textSize = pair(0, lines.size() * TTF_FontLineSkip(font));
	for (const auto& line : lines) {
		int lineWidth;
		TTF_SizeText(font, line.c_str(), &lineWidth, nullptr);
		textSize.x = std::max(textSize.x, lineWidth);
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, textSize.x, textSize.y, 32, SDL_PIXELFORMAT_RGBA32);
	if (!surface) ERROR("Failed to create surface:", SDL_GetError());

	Uint32 colorKey = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
    SDL_SetColorKey(surface, SDL_TRUE, colorKey);

	SDL_Rect textRect = {0, 0, 0, 0};
	for (const auto& line : lines) {
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), colour);
		if (!textSurface) ERROR("Failed to render text:", TTF_GetError());
		textRect.w = textSurface->w;
		textRect.h = textSurface->h;
		SDL_BlitSurface(textSurface, nullptr, surface, &textRect);
		SDL_FreeSurface(textSurface);
		textRect.y += TTF_FontLineSkip(font);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::instance->renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) ERROR("Failed to create texture:", SDL_GetError());

	if (centred) position -= textSize/2;
	SDL_Rect dstRect = {position.x, position.y, textSize.x, textSize.y};
	SDL_RenderCopy(Window::instance->renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);
}

void TextManager::drawText(Text& text, pair position, bool centred) {
	TTF_SetFontStyle(font, text.style);
	drawText(text.text, position, centred, text.colour);
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
}