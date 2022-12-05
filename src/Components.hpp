
#pragma once
#include "utils/utils.hpp"

enum class SpriteSheet;

struct PositionComponent {
    vec position;
};

struct SpriteComponent {
    SpriteSheet spriteSheet;
};

struct CameraComponent {
    int zoom;
};