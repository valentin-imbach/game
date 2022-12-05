
#pragma once
#include "utils/utils.hpp"

enum class SpriteSheet;

enum class TileId : uint8_t {
    NONE,

    GRASS,
    DIRT,

    MAX
};

class Tile {
public:
    Tile(TileId tileId = TileId::NONE);
    TileId tileId;
    pair style = {1, 1};
    static std::array<SpriteSheet, size_t(TileId::MAX)> spriteSheets;
};