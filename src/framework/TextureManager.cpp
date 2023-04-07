
#include "TextureManager.hpp"
#include <SDL2_image/SDL_image.h>
#include "Window.hpp"

SDL_Texture* TextureManager::loadTexture(std::string path) {
	SDL_Surface* tmpSurface = IMG_Load((SPRITE_PATH + path).c_str());
	if (!tmpSurface) {
		ERROR("Failed to load SDL_Texture from", path)
		return nullptr;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::instance->renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	LOG("Texture loaded from", path);
	return tex;
}

void TextureManager::drawTexture(SDL_Texture* texture, pair spos, pair ssize, pair dpos, pair dsize, TextureStyle style) {
	if (!texture) return;
	if (style.centered) dpos -= dsize / 2;
	SDL_Rect srect = {spos.x, spos.y, ssize.x, ssize.y};
	SDL_Rect drect = {dpos.x, dpos.y, dsize.x, dsize.y};
	SDL_SetTextureAlphaMod(texture, style.alpha * 255);
	SDL_SetTextureColorMod(texture, style.tint.r, style.tint.g, style.tint.b);
	SDL_RenderCopyEx(Window::instance->renderer, texture, &srect, &drect, 0, NULL, style.flip);
}

void TextureManager::drawRect(pair position, pair size, SDL_Colour colour, bool centered, bool filled) {
	if (centered) position -= size / 2;
	SDL_Rect rect = {position.x, position.y, size.x, size.y};
	SDL_SetRenderDrawColor(Window::instance->renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_SetRenderDrawBlendMode(Window::instance->renderer, SDL_BLENDMODE_BLEND);
	filled ? SDL_RenderFillRect(Window::instance->renderer, &rect) : SDL_RenderDrawRect(Window::instance->renderer, &rect);
}
