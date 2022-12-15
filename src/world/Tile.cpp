
#include "Tile.hpp"
#include "Sprite.hpp"

Tile::Tile(TileId::value tileId, SpriteStack spriteStack) : tileId(tileId), spriteStack(spriteStack) {}

std::array<SpriteSheet::value, TileId::count> Tile::spriteSheets = {
    SpriteSheet::NONE,
    SpriteSheet::GRASS,
    SpriteSheet::DIRT,
    SpriteSheet::ROCK,
    SpriteSheet::SAND,
    SpriteSheet::WATER,
};