
#pragma once
#include "Item_types.hpp"
#include "Loot.hpp"
#include "utils.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"

struct ResourceTemplate {

	pair size = {1, 1};
	std::vector<SpriteTemplate> spriteTemplates;

	bool solid = true;
	bool opaque = true;

	ItemKind::value toolId = ItemKind::NONE;
	uchar level = 1;

	SoundId::value sound = SoundId::NONE;

	uchar health = 1;

	// SoundId sound;

	LootTable lootTable;

	static std::array<ResourceTemplate, ResourceId::count> templates;
	static void setTemplates();
};

