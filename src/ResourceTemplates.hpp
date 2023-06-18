
#pragma once
#include "Item_types.hpp"
#include "Loot.hpp"
#include "utils.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"

ENUM(ResourceId,
	BASALT_ROCK,
	OBSIDIAN_ROCK,
	GRANITE_ROCK,
	SANDSTONE_ROCK,
	CHALK_ROCK,

	PINE_TREE,
	OAK_TREE,
	ROSEWOOD_TREE,
	ASHWOOD_TREE,
	SPRUCE_TREE,

	BUSH,

	BRANCH,
	PEBBLE,
	GRASS,
	MUSHROOM,

	BASALT_BOULDER
)

struct ResourceTemplate {

	ResourceTemplate() {
		spriteTemplates = std::vector<SpriteTemplate>();
	}

	pair size = {1, 1};
	std::vector<SpriteTemplate> spriteTemplates;

	bool solid = true;
	bool opaque = true;

	ItemKind::value toolId;
	uchar level = 1;

	SoundId::value sound = SoundId::NONE;

	uchar health = 1;

	// SoundId sound;

	LootTable lootTable;

	static std::array<std::unique_ptr<ResourceTemplate>, ResourceId::count> templates;
	static void setTemplates();
};

