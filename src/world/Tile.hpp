
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

ENUM(TileId,
GRASS,
DIRT,
ROCK,
SAND,
WATER)

class Tile {
public:
	Tile(TileId::value tileId, SpriteStack spriteStack);
	TileId::value tileId;
	SpriteStack spriteStack;
	static std::array<SpriteSheet::value, TileId::count> spriteSheets;
};