
#include "Tile.hpp"

Tile::Tile(TileId tileId, SpriteStack spriteStack) : tileId(tileId), spriteStack(spriteStack) {}

std::array<SpriteSheet, size_t(TileId::MAX)> Tile::spriteSheets = {
    SpriteSheet::NONE,
    SpriteSheet::GRASS,
    SpriteSheet::DIRT};