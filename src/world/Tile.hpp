
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

ENUM(TileId,
PLANKS,
GRASS,
DIRT,
MUD,
ROCK,
GRAVEL,
SAND,
WATER)

class Tile {
public:
	Tile(TileId::value tileId);
	TileId::value tileId;
	std::vector<std::pair<TileId::value, Sprite>> sprites;
	static std::array<SpriteSheet::value, TileId::count> spriteSheets;
	static std::array<uint, TileId::count> tileColours;
};