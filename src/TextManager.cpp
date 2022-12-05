
#include "TextManager.hpp"
#include "Window.hpp"

TTF_Font* TextManager::font = nullptr;

void TextManager::Init() {
	if (TTF_Init() != 0) {
		ERROR("Failed to initialize TTF");
		return;
	}
	LOG("TextManager initialized");
	TextManager::loadFont("../assets/fonts/font.ttf", 32);
}

void TextManager::loadFont(const char* path, int size) {
	font = TTF_OpenFont(path, size);
	if (!font) {
		ERROR("Failed to load font from", path)
		return;
	}
	LOG("Font loaded from", path);
}

pair TextManager::textSize(std::string& text) {
	pair size = {0, 0};
	if (TTF_SizeText(font, text.c_str(), &size.x, &size.y)) ERROR("Failed to determine size of string");
	return size;
}

void TextManager::drawText(
    std::string& text, pair position, bool centred, SDL_Color colour) {
	if (text.empty()) return;
	if (!font) {
		ERROR("Trying to render text without font loaded");
		return;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), colour);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::instance->renderer, textSurface);
	pair textSize(textSurface->w, textSurface->h);
	if (centred) position -= (textSize / 2);
	SDL_Rect renderQuad = {position.x, position.y, textSurface->w, textSurface->h};
	SDL_RenderCopy(Window::instance->renderer, texture, NULL, &renderQuad);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(texture);
}
