
#pragma once
#include "TextureManager.hpp"
#include "utils/pair.hpp"

enum class SpriteSheet;

struct PositionComponent {
    pair position;
};

struct SpriteComponent {
    SpriteSheet spriteSheet;
};