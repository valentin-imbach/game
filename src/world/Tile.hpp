
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

ENUM(TileId,
ROCK_WALL,
PLANKS,
GRASS,
SOIL,
DIRT,
MUD,
ROCK,
GRAVEL,
SAND,
WATER)

namespace TileId {
	bool wall(value tileId);
	bool liquid(value tileId);
	float speedMul(value tileId);
	bool walkable(value tileId);
}

class Tile {
public:
	Tile(TileId::value tileId);
	TileId::value tileId;
	std::vector<std::pair<TileId::value, Sprite>> sprites;
	static std::array<SpriteSheet::value, TileId::count> spriteSheets;
	static std::array<uint, TileId::count> tileColours;
};

