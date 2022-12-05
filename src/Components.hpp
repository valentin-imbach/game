
#pragma once
#include "utils/utils.hpp"

enum class SpriteSheet;

struct PositionComponent {
    pair position;
};

struct SpriteComponent {
    SpriteSheet spriteSheet;
};