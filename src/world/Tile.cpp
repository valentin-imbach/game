
#include "Tile.hpp"
#include "Sprite.hpp"

Tile::Tile(TileId::value tileId) : tileId(tileId) {
    sprites = std::vector<std::pair<TileId::value, Sprite>>();
}

std::array<SpriteSheet::value, TileId::count> Tile::spriteSheets = {
    SpriteSheet::NONE,
    SpriteSheet::PLANKS,
    SpriteSheet::GRASS,
    SpriteSheet::DIRT,
    SpriteSheet::MUD,
    SpriteSheet::ROCK,
    SpriteSheet::GRAVEL,
    SpriteSheet::SAND,
    SpriteSheet::WATER,
};