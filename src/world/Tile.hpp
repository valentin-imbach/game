
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

struct GroundTemplate {
	GroundId::value groundId = GroundId::NONE;
	SpriteSheet::value spriteSheet = SpriteSheet::NONE;
	SpriteSheet::value spriteSheet2 = SpriteSheet::NONE;
	uint colour = 0;
	float speed = 1.0f;
	uint frames = 1;
	bool build = true;
	bool walk = true;
	bool liquid = false;

	static void setTemplates();
	static std::array<GroundTemplate, GroundId::count> templates;
};

struct WallTemplate {
	WallId::value wallId = WallId::NONE;
	SpriteSheet::value spriteSheet = SpriteSheet::NONE;
	uint colour = 0;
	uint frames = 1;
	bool build = false;
	bool climb = false;
	bool liquid = false;

	static void setTemplates();
	static std::array<WallTemplate, WallId::count> templates;
};

class Tile {
public:
	Tile(GroundId::value groundId = GroundId::NONE, WallId::value wallId = WallId::NONE);
	GroundId::value groundId;
	WallId::value wallId;
	SpriteStack sprites;
};

