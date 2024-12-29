
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

struct DecorTemplate {

	pair size = {1, 1};
	bool solid = true;
	bool opaque = true;
	std::vector<SpriteTemplate> spriteTemplates;

	static std::array<DecorTemplate, ResourceId::count> templates;
	static void setTemplates(std::filesystem::path root);
};

