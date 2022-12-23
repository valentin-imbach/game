
#pragma once
#include <vector>
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"

class ECS;

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