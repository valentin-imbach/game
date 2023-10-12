
#pragma once
#include "Item_types.hpp"
#include "Loot.hpp"
#include "utils.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"

ENUM(ResourceId,
	BASALT_BOULDER,
	OBSIDIAN_BOULDER,
	GRANITE_BOULDER,
	SANDSTONE_BOULDER,
	CHALK_BOULDER,

	BASALT_ROCK,
	OBSIDIAN_ROCK,
	GRANITE_ROCK,
	SANDSTONE_ROCK,
	CHALK_ROCK,

	BASALT_PEBBLE,
	OBSIDIAN_PEBBLE,
	GRANITE_PEBBLE,
	SANDSTONE_PEBBLE,
	CHALK_PEBBLE,

	OAK_TREE,
	MAPLE_TREE,
	SPRUCE_TREE,
	BIRCH_TREE,

	OAK_BRANCH,
	MAPLE_BRANCH,
	SPRUCE_BRANCH,
	BIRCH_BRANCH,

	OAK_LOG,
	MAPLE_LOG,
	SPRUCE_LOG,
	BIRCH_LOG,

	BUSH,
	GRASS,
	DEAD_BUSH,
	MUSHROOM,
	FLOWER
)

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

