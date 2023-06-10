
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
	MUSHROOM)

struct ResourceTemplate {
	pair anker;
	pair size;
	uint8_t height;

	uint8_t variations = 1;
	bool solid = true;
	bool opaque = true;

	ToolId::value toolId = ToolId::NONE;
	uint8_t level = 1;

	uint8_t health = 1;

	// SoundID sound;

	LootTable lootTable;

	ResourceTemplate(pair anker, pair size = {1, 1}, uint8_t height = 0)
		: anker(anker), size(size), height(height) {}

	static std::array<std::unique_ptr<ResourceTemplate>, ResourceId::count> templates;
	static void setTemplates();
};
