
#pragma once
#include <SDL2/SDL.h>
#include "utils/utils.hpp"

#define SPRITE_PATH "../assets/sprites/"

constexpr int BIT = 16;

enum class SpriteSheet {
    NONE,

    HOLE,
    GRASS,
    DIRT,

    MAX
};

class TextureManager {
public:
    TextureManager() = delete;
    static void Init();
    static void loadSpriteSheets();
    static SDL_Texture* loadTexture(std::string path);
    static void drawTexture(SpriteSheet spriteSheet, pair source, pair size, pair position, int scale = 1, bool centered = false);
private:
    static std::array<SDL_Texture*, int(SpriteSheet::MAX)> spriteSheets;
};