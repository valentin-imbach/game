
#pragma once
#include <SDL2/SDL.h>
#include "utils/utils.hpp"

#define SPRITE_PATH "../assets/sprites/"

enum class SpriteSheet {
    NONE,
    
    HOLE,

    MAX
};

class TextureManager {
public:
    TextureManager() = delete;
    static void loadSpriteSheets();
    static SDL_Texture* loadTexture(std::string path);
    static void drawTexture(SpriteSheet spriteSheet, pair source, pair size, pair position, int scale = 1, bool centered = false);
private:
    static std::array<SDL_Texture*, int(SpriteSheet::MAX)> spriteSheets;
};