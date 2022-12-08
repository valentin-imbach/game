
#pragma once
#include "utils/utils.hpp"
#include "Sprite.hpp"

enum class TileId : uint8_t {
	NONE,

	GRASS,
	DIRT,
	ROCK,
	SAND,
	WATER,

	MAX
};

class Tile {
public:
	Tile(TileId tileId, SpriteStack spriteStack);
	TileId tileId;
	SpriteStack spriteStack;
	static std::array<SpriteSheet, size_t(TileId::MAX)> spriteSheets;
};