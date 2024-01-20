
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"

ENUM(StructureId,
TENT)

struct StructureTemplate {
	SpriteSheet::value spriteSheet;
	pair size = {1, 1};
	std::vector<SpriteTemplate> spriteTemplates;

	static std::array<StructureTemplate, StructureId::count> templates;
	static void setTemplates();
};

