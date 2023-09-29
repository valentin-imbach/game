
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
WATER,
SAND)

struct TileTemplate {
	TileId::value tileId = TileId::NONE;
	SpriteSheet::value spriteSheet = SpriteSheet::NONE;
	uint colour = 0;
	float speed = 1.0f;
	uint frames = 1;
	bool build = true;
	bool walk = true;
	bool liquid = false;
	bool wall = false;

	static void setTemplates();
	static std::array<TileTemplate, TileId::count> templates;
};

class Tile {
public:
	Tile(TileId::value tileId);
	TileId::value tileId;
	std::vector<std::pair<TileId::value, Sprite>> sprites;
};

