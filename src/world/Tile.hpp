
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

struct GroundTemplate {
	GroundId::value groundId = GroundId::NONE;
	SpriteSheet::value spriteSheet = SpriteSheet::NONE;
	uint colour = 0;
	float speed = 1.0f;
	uint frames = 1;
	bool build = true;
	bool walk = true;
	bool liquid = false;

	static void setTemplates(std::filesystem::path root);
	static std::array<GroundTemplate, GroundId::count> templates;
};


class Tile {
public:
	Tile(GroundId::value groundId = GroundId::NONE);
	GroundId::value groundId;
	SpriteStack sprites;
};

