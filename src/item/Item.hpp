
#pragma once
#include <_types/_uint8_t.h>
#include <vector>
#include "utils.hpp"
#include "ECS_types.hpp"

class ECS;

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
	Item(Entity entity) : entity(entity), count(1) {}
	Item(ItemId itemId, int count = 1) : itemId(itemId), count(count) {
		assert(0 < count && count <= MAX_STACK);
	}
	Entity entity = 0;
	ItemId itemId = ItemId::NONE;
	int count = 0;
	
	operator bool() {
		return (entity != 0 || itemId != ItemId::NONE);
	}
	void draw(pair position, int scale, ECS* ecs);
};

enum class ItemAmount {
	ALL,
	HALF,
	ONE
};

struct ItemContainer {
	Item item;

	[[nodiscard]] Item add(Item other, ItemAmount amount = ItemAmount::ALL) {
		int number = MAX_STACK - item.count;
		if (amount == ItemAmount::ALL) number = std::min(number, other.count);
		if (amount == ItemAmount::HALF) number = std::min(number, (other.count + 1) / 2);
		if (amount == ItemAmount::ONE) number = std::min(number, 1);
		if (other.entity) {
			if (!item) {
				item = other;
				other = Item();
			}
		} else if (other.itemId == item.itemId || !item) {
			item.itemId = other.itemId;
			item.count += number;
			other.count -= number;
			assert(other.count >= 0);
			if (other.count == 0) {
				other.itemId = ItemId::NONE;
			}
		}
		return other;
	}
};

class Inventory {
public:
	Inventory(pair size = {0, 0}) : size(size) {
		itemContainers = std::vector<std::vector<ItemContainer>>(size.x, std::vector<ItemContainer>(size.y));
	}
	pair size;
	std::vector<std::vector<ItemContainer>> itemContainers;

	[[nodiscard]] Item add(Item item) {
		for (int y = 0; y < size.x; y++) {
			for (int x = 0; x < size.y; x++) {
				item = itemContainers[x][y].add(item);
			}
		}
		return item;
	}
};