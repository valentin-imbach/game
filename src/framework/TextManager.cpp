
#include "TextManager.hpp"
#include "Window.hpp"
#include <sstream>

TTF_Font* TextManager::font = nullptr;

Text::Text(std::string text, int style, Colour colour)
	: text(text), style(style), colour(colour) {}

void TextManager::Init() {
	if (!TTF_Init()) {
		ERROR("Failed to initialize TTF");
		return;
	}
	LOG("TextManager initialized");
	TextManager::loadFont("font.ttf", 32);
}

void TextManager::cleanup() {
	if (font) TTF_CloseFont(font);
	TTF_Quit();
	LOG("TextManager cleaned up");
}

void TextManager::loadFont(std::string path, int size) {
	font = TTF_OpenFont((FONT_PATH + path).c_str(), size);
	if (!font) {
		ERROR("Failed to load font", path)
		return;
	}
	LOG("Font loaded from", path);
}

pair TextManager::getTextSize(const std::string& text) {
	pair size(0, 0);
	TTF_Text* t = TTF_CreateText(NULL, font, text.c_str(), text.size());
	if (!TTF_GetTextSize(t, &size.x, &size.y)) {
		ERROR("Failed to determine size of string", text);
		return {};
	}
	return size;
}

void TextManager::drawText(std::string& text, vec position, bool centred, Colour colour) {
	if (!font || text.empty()) return;
	std::vector<std::string> lines;
	std::istringstream iss(text);
	std::string line;
	while (std::getline(iss, line)) lines.push_back(line);

	pair textSize = pair(0, lines.size() * TTF_GetFontLineSkip(font));
	for (const auto& line : lines) {
		pair s = getTextSize(line);;
		textSize.x = std::max(textSize.x, s.x);
	}

	SDL_Surface* surface = SDL_CreateSurface(textSize.x, textSize.y, SDL_PIXELFORMAT_RGBA32);
	if (!surface) {
		ERROR("Failed to create surface:", SDL_GetError());
		return;
	}

	Uint32 colourKey = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 0, 0, 0);
    SDL_SetSurfaceColorKey(surface, true, colourKey);

	SDL_Rect textRect = {0, 0, 0, 0};
	for (const auto& line : lines) {
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), line.size(), {colour.r, colour.g, colour.b, colour.a});
		if (!textSurface) {
			ERROR("Failed to render text:", SDL_GetError());
			continue;
		}
		textRect.w = textSurface->w;
		textRect.h = textSurface->h;
		SDL_BlitSurface(textSurface, nullptr, surface, &textRect);
		SDL_DestroySurface(textSurface);
		textRect.y += TTF_GetFontLineSkip(font);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::instance->renderer, surface);
	SDL_DestroySurface(surface);
	if (!texture) {
		ERROR("Failed to create texture:", SDL_GetError());
		return;
	}

	if (centred) position -= textSize/2;
	SDL_FRect dstRect = {float(position.x), float(position.y), float(textSize.x), float(textSize.y)};
	SDL_RenderTexture(Window::instance->renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);
}

void TextManager::drawText(Text& text, vec position, bool centred) {
	TTF_SetFontStyle(font, text.style);
	drawText(text.text, position, centred, text.colour);
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
}