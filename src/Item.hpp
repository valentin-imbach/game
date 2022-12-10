
#pragma once
#include <vector>
#include "utils/utils.hpp"
#include "ECS_types.hpp"

#define MAX_STACK 99

enum class ItemId {
	NONE,

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

	GRASS_FIBRE,

	MAX
};

struct Item {
	Item() = default;
	Item(Entity entity) : entity(entity), stackable(false) {}
	Item(ItemId itemId, uint8_t count = 1) : itemId(itemId), count(count), stackable(true) {}
	bool stackable;
	Entity entity;
	ItemId itemId;
	uint8_t count;
	operator bool() {
		return (entity != 0 || itemId != ItemId::NONE);
	}
};

struct ItemSlot {
	Item item;
	
	Item add(Item other) {
		if (!item) {
			item = other;
			return Item();
		} else if (item.stackable && other.stackable && item.itemId == other.itemId) {
			if (item.count + other.count <= MAX_STACK) {
				item.count += other.count;
				return Item();
			}
			other.count -= (MAX_STACK - item.count);
			item.count = MAX_STACK;
			return other;
		}
		return other;
	}
};

class Inventory {
public:
	Inventory(int width = 0, int height = 0) : width(width), height(height) {
		itemSlots = std::vector<std::vector<ItemSlot>>(width, std::vector<ItemSlot>(height));
	}
	uint8_t width;
	uint8_t height;
	std::vector<std::vector<ItemSlot>> itemSlots;

	[[nodiscard]] Item add(Item item) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				item = itemSlots[x][y].add(item);
			}
		}
		return item;
	}
};