
#include "Tile.hpp"
#include "TextureManager.hpp"

Tile::Tile(TileId tileId) : tileId(tileId) {}

std::array<SpriteSheet, size_t(TileId::MAX)> Tile::spriteSheets = {
    SpriteSheet::NONE,
    SpriteSheet::GRASS,
    SpriteSheet::DIRT
};