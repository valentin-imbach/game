
#include "TextureManager.hpp"
#include "Window.hpp"

#include "SDL3_image/SDL_image.h"

SDL_Texture* TextureManager::lightTexture = nullptr;
SDL_Texture* TextureManager::fogTexture = nullptr;

SDL_Texture* TextureManager::loadTexture(std::string path, bool outline) {
	SDL_Surface* tmpSurface = IMG_Load((SPRITE_PATH + path).c_str());
	if (!tmpSurface) {
		ERROR("Failed to load SDL_Texture from", path)
		return nullptr;
	}
	if (outline) outlineSurface(tmpSurface);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::instance->renderer, tmpSurface);
	SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
	SDL_DestroySurface(tmpSurface);
	LOG("Texture loaded from", path, outline ? "(outlined)" : "");
	return tex;
}

void TextureManager::drawTexture(SDL_Texture* src, SDL_Texture* dst, vec spos, vec ssize, vec dpos, vec dsize, TextureStyle style) {
	if (!src) return;
	if (style.centered) dpos -= dsize / 2;
	SDL_FRect srect = {float(spos.x), float(spos.y), float(ssize.x), float(ssize.y)};
	SDL_FRect drect = {float(dpos.x), float(dpos.y), float(dsize.x), float(dsize.y)};
	SDL_FPoint pivot = {float(dsize.x) / 2 + style.pivot.x, float(dsize.y) / 2 + style.pivot.y};
	SDL_SetRenderTarget(Window::instance->renderer, dst);
	SDL_SetTextureAlphaMod(src, style.alpha * 255);
	SDL_SetTextureColorMod(src, style.tint.r, style.tint.g, style.tint.b);
	SDL_RenderTextureRotated(Window::instance->renderer, src, &srect, &drect, -style.angle * 180 / M_PI, &pivot, style.flip);
}

bool TextureManager::ison(vec point, SDL_Texture* src, SDL_Texture* dst, vec spos, vec ssize, vec dpos, vec dsize, TextureStyle style) {
	if (!src) return false;
	if (style.centered) dpos -= dsize / 2;
	return vec::inside(point, dpos, dsize, false);
}

void TextureManager::drawRect(vec position, vec size, Colour colour, bool centered, bool filled) {
	if (centered) position -= size / 2;
	SDL_FRect rect = {float(position.x), float(position.y), float(size.x), float(size.y)};
	SDL_SetRenderTarget(Window::instance->renderer, NULL);
	SDL_SetRenderDrawColor(Window::instance->renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_SetRenderDrawBlendMode(Window::instance->renderer, SDL_BLENDMODE_BLEND);
	filled ? SDL_RenderFillRect(Window::instance->renderer, &rect) : SDL_RenderRect(Window::instance->renderer, &rect);
}

void TextureManager::drawCirc(vec position, int radius, Colour colour) {
	SDL_SetRenderTarget(Window::instance->renderer, NULL);
	SDL_SetRenderDrawColor(Window::instance->renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_SetRenderDrawBlendMode(Window::instance->renderer, SDL_BLENDMODE_BLEND);

	int x = (radius - 1);
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = (tx - 2 * radius);

	while (x >= y) {
		SDL_RenderPoint(Window::instance->renderer, position.x + x, position.y - y);
		SDL_RenderPoint(Window::instance->renderer, position.x + x, position.y + y);
		SDL_RenderPoint(Window::instance->renderer, position.x - x, position.y - y);
		SDL_RenderPoint(Window::instance->renderer, position.x - x, position.y + y);
		SDL_RenderPoint(Window::instance->renderer, position.x + y, position.y - x);
		SDL_RenderPoint(Window::instance->renderer, position.x + y, position.y + x);
		SDL_RenderPoint(Window::instance->renderer, position.x - y, position.y - x);
		SDL_RenderPoint(Window::instance->renderer, position.x - y, position.y + x);

		if (error <= 0) {
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0) {
			--x;
			tx += 2;
			error += (tx - 2 * radius);
		}
	}
}

SDL_Texture* TextureManager::createTexture(pair size, Colour colour) {
	SDL_Texture* texture = SDL_CreateTexture(Window::instance->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(Window::instance->renderer, texture);
	SDL_SetRenderDrawBlendMode(Window::instance->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Window::instance->renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderClear(Window::instance->renderer);
	return texture;
}

void TextureManager::outlineSurface(SDL_Surface* surface) {
	if (!surface) return;
	if (!SDL_LockSurface(surface)) {
		ERROR("Failed to lock surface");
		return;
	}
	uint* pixels = (uint*)surface->pixels;
	for (int y = 0; y < surface->h; ++y) {
		for (int x = 0; x < surface->w; ++x) {
			uint pixel = pixels[y * (surface->w) + x];
			uchar alpha = (pixel >> 24) & 0xFF;
			if (alpha > 0) continue;

			bool outline = false;
			for (int nx = x-1; nx <= x+1; nx++) {
				for (int ny = y-1; ny <= y+1; ny++) {
					if (nx < 0 || nx >= surface->w || ny < 0 || ny >= surface->h) continue;
					uint pix = pixels[ny * (surface->w) + nx];
					uchar a = (pix >> 24) & 0xFF;
					if (a > 0 && pix != 0xFFFFFFFF) outline = true;
				}
			}
			if (outline) pixels[y * (surface->w) + x] = 0xFFFFFFFF;
		}
	}
}
