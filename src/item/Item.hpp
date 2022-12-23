
#pragma once
#include <vector>
#include "utils.hpp"
#include "ECS_types.hpp"

class ECS;

#define MAX_STACK 99

ENUM(ItemId,
SWORD,
PICK_AXE,
AXE,
SHOVEL,
BOW,
ARROW,

BOOTS,
ROBE,
CAP,
SHIELD,
POTION,
APPLE,

PINE_LOG,
OAK_LOG,
ROSEWOOD_LOG,
ASHWOOD_LOG,
SPRUCE_LOG,
X_LOG,

PINE_PLANK,
OAK_PLANK,
ROSEWOOD_PLANK,
ASHWOOD_PLANK,
SPRUCE_PLANK,
X_PLANK,

PINE_STICK,
OAK_STICK,
ROSEWOOD_STICK,
ASHWOOD_STICK,
SPRUCE_STICK,
X_STICK,

BASALT_COBBLE,
OBSIDIAN_COBBLE,
GRANITE_COBBLE,
SANDSTONE_COBBLE,
CHALK_COBBLE,
Y_COBBLE,

BASALT_PEBBLE,
OBSIDIAN_PEBBLE,
GRANITE_PEBBLE,
SANDSTONE_PEBBLE,
CHALK_PEBBLE,
Y_PEBBLE,

GRASS_FIBRE)

ENUM(ToolId,
PICK_AXE,
AXE,
SHOVEL,
KNIVE)

ENUM(ItemAmount,
ALL,
HALF,
ONE)

struct Item {
	Item() = default;
	Item(Entity entity);
	Item(ItemId::value itemId, int count = 1);
	Entity entity = 0;
	ItemId::value itemId = ItemId::NONE;
	int count = 0;
	
	operator bool();
	void draw(pair position, int scale, ECS* ecs);
};

struct ItemContainer {
	Item item;
	[[nodiscard]] Item add(Item other, ItemAmount::value amount = ItemAmount::ALL);
	void clear();
};

class Inventory {
public:
	pair size;
	Inventory(pair size = {0, 0});
	
	std::vector<std::vector<ItemContainer>> itemContainers;

	[[nodiscard]] Item add(Item item);

	void clear();
};