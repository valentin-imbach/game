
#pragma once
#include "Item_types.hpp"
#include "Loot.hpp"
#include "utils.hpp"

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
	MUSHROOM
)

struct SpriteTemplate {
	pair anker;
	pair size = {1, 1};
	pair offset = {0, 0};
	uint8_t variations = 1;
};

struct ResourceTemplate {

	ResourceTemplate() {
		spriteTemplates = std::vector<SpriteTemplate>();
	}

	pair size = {1, 1};
	std::vector<SpriteTemplate> spriteTemplates;

	bool solid = true;
	bool opaque = true;

	ToolId::value toolId = ToolId::NONE;
	uint8_t level = 1;

	uint8_t health = 1;

	// SoundID sound;

	LootTable lootTable;

	static std::array<std::unique_ptr<ResourceTemplate>, ResourceId::count> templates;
	static void setTemplates();
};

