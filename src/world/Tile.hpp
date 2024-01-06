
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

ENUM(GroundId,
ROCK_WALL,
PLANKS,
GRASS,
SOIL,
DIRT,
MUD,
ROCK,
MUD_WALL,
GRAVEL,
WATER,
SAND)

struct GroundTemplate {
	GroundId::value groundId = GroundId::NONE;
	SpriteSheet::value spriteSheet = SpriteSheet::NONE;
	uint colour = 0;
	float speed = 1.0f;
	uint frames = 1;
	bool build = true;
	bool walk = true;
	bool liquid = false;
	bool wall = false;

	static void setTemplates();
	static std::array<GroundTemplate, GroundId::count> templates;
};

class Tile {
public:
	Tile(GroundId::value groundId, GroundId::value wallId = GroundId::NONE);
	GroundId::value groundId;
	GroundId::value wallId;
	SpriteStack sprites;
};

