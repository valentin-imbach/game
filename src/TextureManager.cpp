
#include "TextureManager.hpp"
#include "utils/logger.hpp"
#include "Window.hpp"
#include <SDL2_image/SDL_image.h>

std::array<SDL_Texture*, int(SpriteSheet::MAX)> TextureManager::spriteSheets = {};

SDL_Texture* TextureManager::loadTexture(std::string path) {
    SDL_Surface* tmpSurface = IMG_Load((SPRITE_PATH + path).c_str());
    if (!tmpSurface) {
        ERROR("Failed to load SDL_Texture from", path)
        return nullptr;
    }
    LOG("Texture loaded from", path);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::instance -> renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return tex;
}

void TextureManager::drawTexture(SpriteSheet spriteSheet, pair source, pair size, pair destination, int scale) {
    SDL_Texture* texture = spriteSheets[int(spriteSheet)];
    if (!texture) return;
    SDL_Rect s = {source.x, source.y, size.x, size.y};
    SDL_Rect d = {destination.x, destination.y, scale * size.x, scale * size.y};
    SDL_RenderCopyEx(Window::instance -> renderer, texture, &s, &d, 0, NULL, SDL_FLIP_NONE);
}

void TextureManager::loadSpriteSheets() {
    spriteSheets[int(SpriteSheet::HOLE)] = loadTexture("hole.png");
}
